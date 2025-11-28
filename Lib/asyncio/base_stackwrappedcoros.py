import reprlib
from . import coroutines


def _stackwrappedcoro_repr_info(stackwrappedcoro):
    info = []

    if stackwrappedcoro._coro:
        coro = coroutines._format_coroutine(stackwrappedcoro._coro)
        info.insert(2, f'coro=<{coro}>')

    return info

@reprlib.recursive_repr()
def _stackwrappedcoro_repr(stackwrappedcoro):
    info = ' '.join(_stackwrappedcoro_repr_info(stackwrappedcoro))
    return f'<{stackwrappedcoro.__class__.__name__} {info}>'
