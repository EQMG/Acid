function _SoundManagerAdd(name, filename)
{
	Module.SoundManagerAdd(Pointer_stringify(name), Pointer_stringify(filename));
}

function _SoundManagerPlay(name)
{
    Module.SoundManagerPlay(Pointer_stringify(name));
}

function _SoundManagerPause(name)
{
    Module.SoundManagerPause(Pointer_stringify(name));
}

function _SoundManagerStop(name)
{
    Module.SoundManagerStop(Pointer_stringify(name));
}

function _SoundManagerLoop(name)
{
    Module.SoundManagerLoop(Pointer_stringify(name));
}

function _SoundManagerSetGain(name, gain)
{
    Module.SoundManagerSetGain(Pointer_stringify(name), gain);
}
