import unittest

class AwaitAnywhereTests(unittest.TestCase):
    def test_await_anywhere(self):
        import asyncio

        def withasync():
            await asyncio.sleep(0.01)
            return 'from withasync()'

        async def asyncentry(testcase):
            return withasync()


        r = asyncio.run(asyncentry(self))
        self.assertEqual(r, 'from withasync()')

    def test_await_disallowed_through_C_call(self):
        import asyncio

        def withasync():
            await asyncio.sleep(0.01)
            return 'from withasync()'
        
        def checknotallowed():
            from collections import defaultdict
            d = defaultdict(withasync)
            return d['a']

        self.assertRaises(RuntimeError, checknotallowed)

    def test_async_for_anywhere(self):
        import asyncio

        async def afor():
            yield 1
            await asyncio.sleep(0.01)
            yield 2
            await asyncio.sleep(0.01)
            yield 3

        def withasync():
            r = []
            async for a in afor():
                r.append(a)
            return r
        
        async def dotest():
            return withasync()
        
        self.assertEqual(asyncio.run(dotest()), [1,2,3])

    def test_async_for_disallowed_through_C_call(self):
        import asyncio

        async def afor():
            yield 1
            await asyncio.sleep(0.01)
            yield 2
            await asyncio.sleep(0.01)
            yield 3

        def withasync():
            r = []
            async for a in afor():
                r.append(a)
            return r
        
        def checknotallowed():
            from collections import defaultdict
            d = defaultdict(withasync)
            return d['a']

        self.assertRaises(RuntimeError, checknotallowed)

    def test_async_with_anywhere(self):
        import asyncio

        class SupportAsyncWith:
            def __init__(self):
                self.entered = False
                self.exited = False

            async def __aenter__(self):
                self.entered = True
                await asyncio.sleep(0.01)
                return self

            async def __aexit__(self, exc_type, exc_value, traceback):
                await asyncio.sleep(0.01)
                self.exited = True

        def withasync():
            async with SupportAsyncWith() as value:
                ...
            return value

        async def dotest():
            return withasync()
        
        value = asyncio.run(dotest())
        self.assertTrue(value.entered)
        self.assertTrue(value.exited)

    def test_async_with_disallowed_through_C_call(self):
        import asyncio

        class SupportAsyncWith:
            def __init__(self):
                self.entered = False
                self.exited = False

            async def __aenter__(self):
                self.entered = True
                await asyncio.sleep(0.01)
                return self

            async def __aexit__(self, exc_type, exc_value, traceback):
                await asyncio.sleep(0.01)
                self.exited = True

        def withasync():
            async with SupportAsyncWith() as value:
                ...
            return value
        
        def checknotallowed():
            from collections import defaultdict
            d = defaultdict(withasync)
            return d['a']

        self.assertRaises(RuntimeError, checknotallowed)
