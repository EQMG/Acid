
function _audioAdd(name, filename)
{
	Module.audioAdd(Pointer_stringify(name), Pointer_stringify(filename));
}

function _audioPlay(name)
{
    Module.audioPlay(Pointer_stringify(name));
}

function _audioPause(name)
{
    Module.audioPause(Pointer_stringify(name));
}

function _audioStop(name)
{
    Module.audioStop(Pointer_stringify(name));
}

function _audioLoop(name)
{
    Module.audioLoop(Pointer_stringify(name));
}

function _audioSetGain(name, gain)
{
    Module.audioSetGain(Pointer_stringify(name), gain);
}