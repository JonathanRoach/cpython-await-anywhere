import unittest

class AwaitAnywhereTests(unittest.TestCase):
    def test_await_anywhere(self):
        import asyncio

        def withasync():
            await asyncio.sleep(0.01)

        async def asyncentry():
            withasync()

        asyncio.run(asyncentry())
