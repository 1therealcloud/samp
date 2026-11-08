#ifndef STATIC_ASSERT_H
#define STATIC_ASSERT_H

#define STATIC_ASSERT_CAT_IMPL(a, b) a##b
#define STATIC_ASSERT_CAT(a, b)      STATIC_ASSERT_CAT_IMPL(a, b)

#define STATIC_ASSERT(cond) \
    typedef char STATIC_ASSERT_CAT(static_assert_failed_, __COUNTER__)[(cond) ? 1 : -1]

#endif // STATIC_ASSERT_H
