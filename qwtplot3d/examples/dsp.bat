REM set QMAKESPEC=win32-msvc.net
cd mesh2
qmake mesh2.pro -o mesh2.dsp
cd ../simpleplot
qmake simpleplot.pro -o simpleplot.dsp
cd ../freemesh
qmake freemesh.pro -o freemesh.dsp
cd ../autoswitch
qmake autoswitch.pro -o autoswitch.dsp
cd ../enrichments
qmake enrichments.pro -o enrichments.dsp
cd ../axes
qmake axes.pro -o axes.dsp
