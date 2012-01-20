# -*- python -*-
# $Header: /nfs/slac/g/glast/ground/cvs/GuiSvc/SConscript,v 1.13 2012/01/19 20:23:18 jrb Exp $
# Authors: T.Burnett <tburnett@u.washington.edu>
# Version: GuiSvc-03-09-02
Import('baseEnv')
Import('listFiles')
Import('packages')
progEnv = baseEnv.Clone()
libEnv = baseEnv.Clone()

libEnv.Tool('addLinkDeps', package='GuiSvc', toBuild='component')
GuiSvc = libEnv.ComponentLibrary('GuiSvc', listFiles(['src/*.cxx']))

progEnv.Tool('GuiSvcLib')
testsrcs = listFiles(['src/test/*.cxx'])

GuiTest = progEnv.GaudiProgram('GuiTest',testsrcs,
			       test = 0, package='GuiSvc')

progEnv.Tool('registerTargets', package = 'GuiSvc',
             libraryCxts = [[GuiSvc, libEnv]],
             testAppCxts = [[GuiTest, progEnv]],
             includes = listFiles(['GuiSvc/*.h']),
	     jo = ['src/test/jobOptions.txt'])




