# -*- python -*-
# $Header$
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: GuiSvc-03-08-01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('GuiSvcLib', depsOnly = 1)
GuiSvc = libEnv.SharedLibrary('GuiSvc', listFiles(['src/*.cxx']))

progEnv.Tool('GuiSvcLib')
progEnv.Tool('guiLib')
GuiTest = progEnv.GaudiProgram('GuiTest', listFiles(['src/test/*.cxx']), test = 1)

progEnv.Tool('registerObjects', package = 'GuiSvc', libraries = [GuiSvc], testApps = [GuiTest], includes = listFiles(['GuiSvc/*.h']))
