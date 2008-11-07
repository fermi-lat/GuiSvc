# $Header: /nfs/slac/g/glast/ground/cvs/GlastRelease-scons/GuiSvc/GuiSvcLib.py,v 1.2 2008/07/11 00:32:27 glast Exp $
def generate(env, **kw):
    if not kw.get('depsOnly', 0):
        env.Tool('addLibrary', library = ['GuiSvc'])
    env.Tool('guiLib')
    env.Tool('addLibrary', library = env['gaudiLibs'])
def exists(env):
    return 1;
