# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/GuiSvc/SConscript,v 1.7 2009/11/06 23:25:39 jrb Exp $
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: GuiSvc-03-08-02
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('GuiSvcLib', depsOnly = 1)
GuiSvc = libEnv.StaticLibrary('GuiSvc', listFiles(['src/*.cxx']))

progEnv.Tool('GuiSvcLib')
progEnv.Tool('addLibrary', library = ['dl'])
if baseEnv['PLATFORM'] != 'win32':
	progEnv.AppendUnique(LINKFLAGS = '-u GuiSvc_loadRef')
GuiTest = progEnv.GaudiProgram('GuiTest', listFiles(['src/test/*.cxx']), test = 1)

progEnv.Tool('registerTargets', package = 'GuiSvc',
             libraryCxts = [[GuiSvc, libEnv]],
             testAppCxts = [[GuiTest, progEnv]],
             includes = listFiles(['GuiSvc/*.h']))




