# Fonts

The Esc menu (src/ui/pause_menu.cpp) uses **Poppins** Medium, Bold and Bold Italic
by the Poppins Project Authors (https://github.com/itfoundry/Poppins), under
the SIL Open Font License 1.1 (OFL.txt).

The files here are Latin subsets (Basic Latin, Latin-1 and common punctuation)
made with fontTools:

    pyftsubset Poppins-Medium.ttf --unicodes="U+0020-007E,U+00A0-00FF,U+2013,U+2014,U+2018,U+2019,U+201C,U+201D,U+2022,U+2026,U+00D7" --layout-features='kern,liga' --output-file=Poppins-Medium-Latin.ttf

and are embedded in the exe through src/ui/menu_fonts.h, generated from them
(each file as a byte array).
