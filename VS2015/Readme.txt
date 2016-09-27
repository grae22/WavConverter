~~~~~~~~~~~~~~
 WavConverter 
~~~~~~~~~~~~~~

Building the VS2015 solution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

x Add the following environment vars:
  x BoostInclude - path to the Boost include folder, e.g. c:\boost\boost_1_61_0\
  x BoostLibs - path to the Boost libs folder, e.g. c:\boost\boost_1_61_0\libs\

Notes
~~~~~

x For now, I'm only focussing on a working 64 bit version. A 32 bit version will require
  some changes - for example, the KWav data buffer can exceed a 32 bit size.