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

    def test_await_in_ops(self):
        import asyncio

        # has no overrides
        class WithXorBase:
            ...

        def optest(op):
            def r(self, other):
                if isinstance(other, WithXorBase):
                    other = 'base'
                await asyncio.sleep(0.01)
                return f'{other} {op}'
            return r

        def ioptest(op):
            def r(self, other):
                if isinstance(other, WithXorBase):
                    other = 'base'
                await asyncio.sleep(0.01)
                self.v = f'{other} {op}'
                return self
            return r

        class WithXor(WithXorBase):
            # ops and rops, eg v - w and w - v as seen from v
            __and__ = optest('and')
            __rand__ = optest('rand')
            __floordiv__ = optest('floordiv')
            __rfloordiv__ = optest('rfloordiv')
            __lshift__ = optest('lshift')
            __rlshift__ = optest('rlshift')
            __matmul__ = optest('matmul')
            __rmatmul__ = optest('rmatmul')
            __mod__ = optest('mod')
            __rmod__ = optest('rmod')
            __or__ = optest('or')
            __ror__ = optest('ror')
            __rshift__ = optest('rshift')
            __rrshift__ = optest('rrshift')
            __sub__ = optest('sub')
            __rsub__ = optest('rsub')
            __truediv__ = optest('truediv')
            __rtruediv__ = optest('rtruediv')
            __xor__ = optest('xor')
            __rxor__ = optest('rxor')

            # iops, eg v -= w
            __iadd__ = ioptest('iadd')
            __iand__ = ioptest('iand')
            __ifloordiv__ = ioptest('ifloordiv')
            __ilshift__ = ioptest('ilshift')
            __imatmul__ = ioptest('imatmul')
            __imul__ = ioptest('imul')
            __imod__ = ioptest('imod')
            __ior__ = ioptest('ior')
            __ipow__ = ioptest('ipow')
            __irshift__ = ioptest('irshift')
            __isub__ = ioptest('isub')
            __itruediv__ = ioptest('itruediv')
            __ixor__ = ioptest('ixor')

        def doptest(op):
            def r(self, other):
                if issubclass(other.__class__, WithXorBase):
                    other = other.__class__.__name__
                await asyncio.sleep(0.01)
                return f'{other} {op}'
            return r

        def dioptest(op):
            def r(self, other):
                if isinstance(other, WithXorBase):
                    other = other.__class__.__name__
                await asyncio.sleep(0.01)
                self.v = f'{other} {op}'
                return self
            return r

        class WithXorDeriv(WithXor):
            # ops and rops, eg v - w and w - v as seen from v
            __and__ = doptest('dand')
            __rand__ = doptest('drand')
            __floordiv__ = doptest('dfloordiv')
            __rfloordiv__ = doptest('drfloordiv')
            __lshift__ = doptest('dlshift')
            __rlshift__ = doptest('drlshift')
            __matmul__ = doptest('dmatmul')
            __rmatmul__ = doptest('drmatmul')
            __mod__ = doptest('dmod')
            __rmod__ = doptest('drmod')
            __or__ = doptest('dor')
            __ror__ = doptest('dror')
            __rshift__ = doptest('drshift')
            __rrshift__ = doptest('drrshift')
            __sub__ = doptest('dsub')
            __rsub__ = doptest('drsub')
            __truediv__ = doptest('dtruediv')
            __rtruediv__ = doptest('drtruediv')
            __xor__ = doptest('dxor')
            __rxor__ = doptest('drxor')

            # iops, eg v -= w
            __iadd__ = dioptest('iadd')
            __iand__ = dioptest('iand')
            __ifloordiv__ = dioptest('ifloordiv')
            __ilshift__ = dioptest('ilshift')
            __imatmul__ = dioptest('imatmul')
            __imul__ = dioptest('imul')
            __imod__ = dioptest('imod')
            __ior__ = dioptest('ior')
            __ipow__ = dioptest('ipow')
            __irshift__ = dioptest('irshift')
            __isub__ = dioptest('isub')
            __itruediv__ = dioptest('itruediv')
            __ixor__ = dioptest('ixor')


        async def dotest1():
            w = WithXor()
            return ((w & 1)
                + (w // 2)
                + (w << 3)
                + (w @ 4)
                + (w % 5)
                + (w | 6)
                + (w >> 7)
                + (w - 8)
                + (w / 9)
                + (w ^ 10)
                + (1 & w)
                + (2 // w)
                + (3 << w)
                + (4 @ w)
                + (5 % w)
                + (6 | w)
                + (7 >> w)
                + (8 - w)
                + (9 / w)
                + (10 ^ w))

        # cls <op> number and number <op> cls
        self.assertEqual(asyncio.run(dotest1()), '1 and'
            +'2 floordiv'
            +'3 lshift'
            +'4 matmul'
            +'5 mod'
            +'6 or'
            +'7 rshift'
            +'8 sub'
            +'9 truediv'
            +'10 xor'
            +'1 rand'
            +'2 rfloordiv'
            +'3 rlshift'
            +'4 rmatmul'
            +'5 rmod'
            +'6 ror'
            +'7 rrshift'
            +'8 rsub'
            +'9 rtruediv'
            +'10 rxor')

        # cls <op> base and base <op> cls
        async def dotest2():
            v = WithXorBase()
            w = WithXor()

            return ((w & v)
                + (w // v)
                + (w << v)
                + (w @ v)
                + (w % v)
                + (w | v)
                + (w >> v)
                + (w - v)
                + (w / v)
                + (w ^ v)
                + (v & w)
                + (v // w)
                + (v << w)
                + (v @ w)
                + (v % w)
                + (v | w)
                + (v >> w)
                + (v - w)
                + (v / w)
                + (v ^ w))

        self.assertEqual(asyncio.run(dotest2()), 'base and'
            +'base floordiv'
            +'base lshift'
            +'base matmul'
            +'base mod'
            +'base or'
            +'base rshift'
            +'base sub'
            +'base truediv'
            +'base xor'
            +'base rand'
            +'base rfloordiv'
            +'base rlshift'
            +'base rmatmul'
            +'base rmod'
            +'base ror'
            +'base rrshift'
            +'base rsub'
            +'base rtruediv'
            +'base rxor')

        # cls <op> deriv and deriv <op> cls
        async def dotest3():
            v = WithXorDeriv()
            w = WithXor()

            return ((w & v)
                + (w // v)
                + (w << v)
                + (w @ v)
                + (w % v)
                + (w | v)
                + (w >> v)
                + (w - v)
                + (w / v)
                + (w ^ v)
                + (v & w)
                + (v // w)
                + (v << w)
                + (v @ w)
                + (v % w)
                + (v | w)
                + (v >> w)
                + (v - w)
                + (v / w)
                + (v ^ w))

        self.assertEqual(asyncio.run(dotest3()), 'WithXor drand'
            +'WithXor drfloordiv'
            +'WithXor drlshift'
            +'WithXor drmatmul'
            +'WithXor drmod'
            +'WithXor dror'
            +'WithXor drrshift'
            +'WithXor drsub'
            +'WithXor drtruediv'
            +'WithXor drxor'
            +'WithXor dand'
            +'WithXor dfloordiv'
            +'WithXor dlshift'
            +'WithXor dmatmul'
            +'WithXor dmod'
            +'WithXor dor'
            +'WithXor drshift'
            +'WithXor dsub'
            +'WithXor dtruediv'
            +'WithXor dxor')

        # cls <iop> number
        async def dotest4():
            r = ''
            v = WithXor()

            v += 1
            r += v.v
            v &= 2
            r += v.v
            v //= 3
            r += v.v
            v <<= 4
            r += v.v
            v @= 5
            r += v.v
            v *= 6
            r += v.v
            v |= 7
            r += v.v
            v >>= 9
            r += v.v
            v -= 10
            r += v.v
            v /= 11
            r += v.v
            v ^= 12
            r += v.v

            return r
        
        self.assertEqual(asyncio.run(dotest4()), '1 iadd'
            + '2 iand'
            + '3 ifloordiv'
            + '4 ilshift'
            + '5 imatmul'
            + '6 imul'
            + '7 ior'
            + '9 irshift'
            + '10 isub'
            + '11 itruediv'
            + '12 ixor')
