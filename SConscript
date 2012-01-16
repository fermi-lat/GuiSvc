# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/SConscript,v 1.11 2011/12/12 22:48:56 heather Exp $
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: GuiSvc-03-09-01
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='GuiSvc', toBuild='static')
GuiSvc = libEnv.StaticLibrary('GuiSvc', listFiles(['src/*.cxx']))

progEnv.Tool('GuiSvcLib')
progEnv.Tool('addLibrary', library = ['dl'])
if baseEnv['PLATFORM'] != 'win32':
	progEnv.AppendUnique(LINKFLAGS = '-u GuiSvc_loadRef')
GuiTest = progEnv.GaudiProgram('GuiTest', listFiles(['src/test/*.cxx']),
			       test = 1, package='GuiSvc')

progEnv.Tool('registerTargets', package = 'GuiSvc',
             libraryCxts = [[GuiSvc, libEnv]],
             testAppCxts = [[GuiTest, progEnv]],
             includes = listFiles(['GuiSvc/*.h']),
	     jo = ['src/test/jobOptions.txt'])




