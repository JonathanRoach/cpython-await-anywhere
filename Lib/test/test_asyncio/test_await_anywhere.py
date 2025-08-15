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
            return r + [a async for a in afor()]
        
        async def dotest():
            return withasync()
        
        self.assertEqual(asyncio.run(dotest()), [1, 2, 3, 1, 2, 3])

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

    def test_await_through_property(self):
        import asyncio

        # tests get...
        class HasAwaitInProperty:
            @property
            def propertywithawait(self):
                await asyncio.sleep(0.01)
                return 'awaitdone'
            
            @propertywithawait.setter
            def propertywithawait(self, value):
                await asyncio.sleep(0.01)
                self._propertywithawait = value

            @propertywithawait.deleter
            def propertywithawait(self):
                await asyncio.sleep(0.01)
                del self._propertywithawait
        
        async def dotest1():
            return HasAwaitInProperty().propertywithawait

        self.assertEqual(asyncio.run(dotest1()), 'awaitdone')
        # ...tests get

        # tests set
        async def dotest2():
            h = HasAwaitInProperty()
            h.propertywithawait = 'awaitdone'
            return h._propertywithawait

        self.assertEqual(asyncio.run(dotest2()), 'awaitdone')
        # ...tests set

        # tests delete
        async def dotest3():
            h = HasAwaitInProperty()
            h.propertywithawait = 'awaitdone'
            del h._propertywithawait
            return hasattr(h, '_propertywithawait')

        self.assertEqual(asyncio.run(dotest3()), False)
        # ...tests delete

    def test_await_through_descriptor(self):
        import asyncio

        # tests __get__ on its own...
        class DescriptorWithGetWithAWait:
            def __get__(self, obj, objtype=None):
                await asyncio.sleep(0.01)
                return 'awaitdone'

        class HasAwaitInGetDescriptor:
            descriptorvalue = DescriptorWithGetWithAWait()

        async def dotest1():
            return HasAwaitInGetDescriptor().descriptorvalue
        
        self.assertEqual(asyncio.run(dotest1()), 'awaitdone')
        # ...tests __get__ on its own

        # tests __get__ with a __set__ present...
        class DescriptorWithGetSetWithAWait:
            def __get__(self, obj, objtype=None):
                await asyncio.sleep(0.01)
                return 'awaitdone'

            def __set__(self, obj, value):
                ...

        class HasAwaitInGetSetDescriptor:
            descriptorvalue = DescriptorWithGetSetWithAWait()

        async def dotest2():
            return HasAwaitInGetSetDescriptor().descriptorvalue

        self.assertEqual(asyncio.run(dotest2()), 'awaitdone')
        # ...tests __get__ with a __set__ present

        # tests __set__ with a __get__ present...
        class DescriptorWithGetSetWithAWait:
            def __get__(self, obj, objtype=None):
                await asyncio.sleep(0.01)
                return 'awaitdone'

            def __set__(self, obj, value):
                await asyncio.sleep(0.01)
                obj._descriptorvalue = value
            
            def __delete__(self, obj):
                await asyncio.sleep(0.01)
                del obj._descriptorvalue

        class HasAwaitInGetSetDescriptor:
            descriptorvalue = DescriptorWithGetSetWithAWait()

        async def dotest2():
            h = HasAwaitInGetSetDescriptor()
            h.descriptorvalue = 'awaitdone'
            return h._descriptorvalue

        self.assertEqual(asyncio.run(dotest2()), 'awaitdone')
        # ...tests __set__ with a __get__ present

        # tests __delete__ ...
        async def dotest3():
            h = HasAwaitInGetSetDescriptor()
            h.descriptorvalue = 'awaitdone'
            del h.descriptorvalue
            return hasattr(h, '_descriptorvalue')

        self.assertEqual(asyncio.run(dotest3()), False)
        # ...tests __delete__

    def test_await_through__getattr__getattribute__(self):
        import asyncio

        class HasAwaitIn__getattr__:
            def __getattr__(self, name):
                if name == 'awaitvalue':
                    await asyncio.sleep(0.01)
                    return 'awaitdone'
                raise AttributeError(name=name)

        async def dotest1():
            return HasAwaitIn__getattr__().awaitvalue
        
        self.assertEqual(asyncio.run(dotest1()), 'awaitdone')

        class HasAwaitIn__getattribute__:
            def __getattribute__(self, name):
                if name == 'awaitvalue':
                    await asyncio.sleep(0.01)
                    return 'awaitdone'
                return super().__getattribute__(self, name)

        async def dotest2():
            return HasAwaitIn__getattribute__().awaitvalue
        
        self.assertEqual(asyncio.run(dotest2()), 'awaitdone')

        class HasAwaitIn__getattribute__and__getattr__:
            def __getattribute__(self, name):
                if name == 'awaitvalue':
                    await asyncio.sleep(0.01)
                    return 'awaitdone'
                if name == 'awaitvalue2':
                    await asyncio.sleep(0.01)
                    raise AttributeError(name=name)
                return super().__getattribute__(self, name)

            def __getattr__(self, name):
                if name == 'awaitvalue':
                    await asyncio.sleep(0.01)
                    return 'awaitdone'
                if name == 'awaitvalue2':
                    await asyncio.sleep(0.01)
                    return 'awaitdone2'
                raise AttributeError(name=name)

        async def dotest2():
            return HasAwaitIn__getattribute__and__getattr__().awaitvalue + HasAwaitIn__getattribute__and__getattr__().awaitvalue2
        
        self.assertEqual(asyncio.run(dotest2()), 'awaitdoneawaitdone2')

    def test_await_through__setattr__(self):
        import asyncio

        class HasAwaitIn__setattr__:
            def __setattr__(self, name, value):
                if name == 'awaitvalue':
                    await asyncio.sleep(0.01)
                    self.aval = value
                super().__setattr__(name, value)

        async def dotest1():
            ob = HasAwaitIn__setattr__()
            ob.awaitvalue = 'awaitdone'
            return ob.aval
        
        self.assertEqual(asyncio.run(dotest1()), 'awaitdone')
