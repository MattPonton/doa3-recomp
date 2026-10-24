"""Inspect a bounded dispatcher audit. Reports observed writes, not replay success.

Includes unconverted tasks and the inter-frame gap. Async clock/APU writers
are not masked out or attributed exclusively to the currently running task.
"""
import argparse
import json
from pathlib import Path
import struct
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent / "recomp"))
from rollback_tasks import source_digest, TASKS
from rollback_boundaries import PHASES

HEADER = struct.Struct("<8I")
COUNTS = struct.Struct("<400I")
RAM = 128 * 1024 * 1024
PAGE = 4096


def exact(stream, count):
    data = stream.read(count)
    if len(data) != count:
        raise ValueError("Truncated dispatcher audit")
    return data


def inspect(path, expected_digest):
    records = []
    state = bytearray(RAM)
    prior_counts = None
    prior_presents = 0
    frame = 0
    active = False
    pending = None
    last_slot = -1
    last_phase = -1
    with path.open("rb") as stream:
        magic, version, memory, offset, page, limit, counters, reserved = HEADER.unpack(exact(stream, 32))
        if version not in (1,2) or (magic, memory, page, limit, counters, reserved) != (0x31464252, RAM, PAGE, 2, 400, 0):
            raise ValueError("Unsupported dispatcher audit header")
        digest = exact(stream, 32)
        if digest != expected_digest:
            raise ValueError("Capture and current translation sources differ")
        while True:
            fields = HEADER.unpack(exact(stream, 32))
            if not any(fields):
                if active or pending or frame != limit or stream.read(1):
                    raise ValueError("Incomplete dispatcher audit or trailing bytes")
                break
            kind, index, slot, routine, count, presents, mode, phase = fields
            if index != frame or frame >= limit or count > RAM // PAGE or mode != 7 or phase != 1:
                raise ValueError("Invalid dispatcher record")
            if kind == 1:
                if active or slot != 0xFFFFFFFF or routine:
                    raise ValueError("Invalid frame start")
                if version==2 and frame and last_phase!=len(PHASES)-1:
                    raise ValueError("Incomplete outer-loop phase sequence")
                active = True
                last_slot = -1
                label = "initial_image" if not records else "between_dispatcher_passes"
            elif kind == 2:
                if not active or pending or not last_slot < slot < 18 or not routine:
                    raise ValueError("Invalid task start")
                pending = (slot, routine)
                last_slot = slot
                label = "dispatcher_before_task"
            elif kind == 3:
                if not active or pending != (slot, routine):
                    raise ValueError("Invalid task end")
                pending = None
                label = "task"
            elif kind == 4:
                if not active or pending or last_slot < 0 or slot != 0xFFFFFFFF or routine:
                    raise ValueError("Invalid frame end")
                active = False
                frame += 1
                last_phase = -1
                label = "dispatcher_end"
            elif kind == 5:
                if (version!=2 or active or pending or frame!=1 or slot!=0xFFFFFFFF or
                        last_phase+1>=len(PHASES) or routine!=PHASES[last_phase+1]):
                    raise ValueError("Invalid outer-loop phase sequence")
                last_phase+=1
                label = "outer_loop_phase"
            else:
                raise ValueError("Unknown record kind")
            current_counts = COUNTS.unpack(exact(stream, COUNTS.size))
            pages = []
            last_page = -1
            changed_bytes = 0
            for _ in range(count):
                number = struct.unpack("<I", exact(stream, 4))[0]
                if not last_page < number < RAM // PAGE:
                    raise ValueError("Invalid or unsorted page index")
                last_page = number
                data = exact(stream, PAGE)
                address = number * PAGE
                old = state[address:address + PAGE]
                differences = [n for n, (a, b) in enumerate(zip(old, data)) if a != b]
                if not differences:
                    raise ValueError("Redundant page delta")
                pages.append(dict(address=f"{address:08X}", bytes=len(differences),
                                  first=f"{address + differences[0]:08X}",
                                  last=f"{address + differences[-1]:08X}"))
                changed_bytes += len(differences)
                state[address:address + PAGE] = data
            calls = {} if prior_counts is None else {
                str(i): (new - old) & 0xFFFFFFFF
                for i, (old, new) in enumerate(zip(prior_counts, current_counts)) if new != old}
            since = (f"{records[-1]['kind']}:{records[-1]['routine']}" if records else "initial")
            records.append(dict(frame=index, kind=label, slot=slot, routine=f"{routine:08X}", since=since,
                                explicit_continuation=routine in TASKS,
                                changed_bytes=changed_bytes, pages=pages, kernel_calls=calls,
                                presentations=0 if prior_counts is None else (presents - prior_presents) & 0xFFFFFFFF))
            prior_counts, prior_presents = current_counts, presents
    tasks = {}
    for record in records:
        if record["kind"] != "task":
            continue
        key = record["routine"]
        item = tasks.setdefault(key, dict(explicit_continuation=record["explicit_continuation"],
                                          calls=0, changed_bytes=0, kernel_calls=0, presentations=0, pages=set()))
        item["calls"] += 1
        item["changed_bytes"] += record["changed_bytes"]
        item["kernel_calls"] += sum(record["kernel_calls"].values())
        item["presentations"] += record["presentations"]
        item["pages"].update(p["address"] for p in record["pages"])
    core_pages = set().union(*(t["pages"] for t in tasks.values() if t["explicit_continuation"]))
    for item in tasks.values():
        item["pages_also_changed_during_core_tasks"] = sorted(item["pages"] & core_pages)
        item["pages"] = sorted(item["pages"])
    return dict(schema=version, frames=frame, source_digest=digest.hex(), guest_offset=f"{offset:08X}",
                warning="Observed boundary writes include asynchronous writers. Page overlap is not proof of a dependency. This is not a replay test or restorable snapshot.",
                tasks=tasks, records=records)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("capture", type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--gen", type=Path, default=Path(__file__).resolve().parents[1] / "src/game/recomp/gen")
    args = parser.parse_args()
    try:
        report = inspect(args.capture, source_digest(args.gen))
    except (OSError, ValueError) as error:
        parser.exit(2, f"Dispatcher audit rejected: {error}\n")
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print("routine   calls  explicit  changed bytes  kernel calls  presents")
    for routine, task in report["tasks"].items():
        print(f"{routine} {task['calls']:5} {str(task['explicit_continuation']):>9} {task['changed_bytes']:14} {task['kernel_calls']:13} {task['presentations']:9}")
    for record in report['records']:
        if record['kind']=='outer_loop_phase':
            print(f"gap {record['since']} -> {record['routine']}: {record['changed_bytes']} changed RAM bytes, "
                  f"{sum(record['kernel_calls'].values())} kernel calls, {record['presentations']} presents")
    print(f"Complete audit: {report['frames']} dispatcher passes; {len(report['tasks'])} task routines.")
    print(report["warning"])


if __name__ == "__main__":
    main()
