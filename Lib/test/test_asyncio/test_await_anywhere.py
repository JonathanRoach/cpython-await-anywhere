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
