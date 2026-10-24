"""Explicit direct suspension graphs for the 3.1 match and game-flow tasks.

Synchronous helper execution keeps the production native path. Any unexpected
native scheduler handoff invalidates the task checkpoint and capture; it must
not be treated as a successful explicit continuation.
"""
import re
try:
    from .rollback_nested import generate
except ImportError:
    from rollback_nested import generate

ROOTS = (0x87420, 0x72070)
MATCH_ROUTINES = frozenset(int(x,16) for x in """
5B140 61390 72070 72980 72F50 72FE0 73540 73700 73870 740D0 74A30 75A40
78120 782E0 786E0 78A60 78B50 78DB0 792B0 792D1 795B0 7A370 7A8D0 7A980
7AB6E 7AE20 7B380 7B3D0 7B960 7BB70 7BD50 7C020 7C0A0 7C340 7C900 7CAC0
7CDB0 7D910 7DAB0 7E590 7E980 85120 856F0 85820
""".split())
FLOW_ROUTINES = frozenset(int(x,16) for x in """
20C80 20DE0 525C0 52DA0 61390 69680 6D790 6D810 6D8A0 6D8E0 6DC10 6DE30
7CDB0 7E590 83600 85120 85820 873A0 87420 87620 87A50 87D00
87EB0 88000 88570 885E0 88E10 88F43 89000 89240 89820 89AC0 8A440 8A5F0
8AFD0 8B279 8B280 18A430
""".split())
# A47E0 initializes movie playback, including native CRI virtual calls and a
# frame wait. It is deliberately NOT a restorable routine. Keep normal native
# execution; the scheduler invalidates checkpoints/cancels recording if it
# suspends. Captures can begin again only after the entire native step returns.
NATIVE_BOUNDARIES = frozenset({0xA47E0})


def suspension_body(va, body):
    if va != 0x496D0:
        return body
    # The lifter includes the next task's loop after this leaf's final ret.
    # Prove that no entry path can reach it before excluding it from the call
    # graph. Do not change the emitted/native body or discard reachable calls.
    marker = "loc_00049795: ;"
    if body.count(marker) != 1:
        raise ValueError("496D0 unreachable tail changed")
    prefix = body.split(marker)[0]
    clean = re.sub(r"/\*.*?\*/|//[^\n]*", "", prefix, flags=re.S).strip()
    labels = re.findall(r"^loc_([0-9A-F]{8}): ;", clean, re.M)
    targets = re.findall(r"\bgoto loc_([0-9A-F]{8});", clean)
    if (not labels or labels[0] != "000496D0" or
            not set(targets) <= set(labels) or
            len(targets) != len(re.findall(r"\bgoto\b", clean)) or
            re.search(r"\b(?:sub_\w+|RECOMP_\w+)\s*\(", clean) or
            not clean.endswith("esp += 4; return;")):
        raise ValueError("496D0 leaf return/branch proof changed")
    return prefix


def suspension_graph(bodies, manual, root):
    edges={va:{int(x,16) for x in re.findall(r"\bsub_([0-9A-F]{8})\(",suspension_body(va,body))}
           for va,body in bodies.items()}
    seen=set(); pending=[root]
    while pending:
        va=pending.pop()
        if va in seen or va in NATIVE_BOUNDARIES or (va in manual and va!=root): continue
        seen.add(va)
        pending.extend(edges.get(va,set())-{0x1C65D8})
    selected={va for va in seen if 0x1C65D8 in edges.get(va,set())}
    while True:
        parents={va for va in seen if edges.get(va,set())&selected}
        if parents<=selected: break
        selected|=parents
    return selected, edges


def generate_match(bodies, manual):
    selected=set()
    for root,expected in ((0x72070,MATCH_ROUTINES),(0x87420,FLOW_ROUTINES)):
        found,edges=suspension_graph(bodies,manual,root)
        if found!=expected:
            raise ValueError(f"{root:08X} suspension graph changed: added={found-expected}, removed={expected-found}")
        selected |= found
    synchronous=set().union(*(edges[va] for va in selected))-selected-{0x1C65D8}
    return generate({va:bodies[va] for va in selected},synchronous)
