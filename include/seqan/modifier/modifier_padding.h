// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2016, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Rene Rahn <rene.rahn@fu-berlin.de>
// ==========================================================================

#ifndef INCLUDE_SEQAN_MODIFIER_MODIFIER_PADDING_H_
#define INCLUDE_SEQAN_MODIFIER_MODIFIER_PADDING_H_

namespace seqan
{

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================
// --------------------------------------------------------------------------
// Class ModPad Iterator
// --------------------------------------------------------------------------

struct ModPadding_;
using ModPadding = Tag<ModPadding_>;

/*!
 * @class ModPaddingIterator
 * @extends ModifiedIterator
 * @headerfile <seqan/modifier.h>
 * @brief Pad characters beyond the end of a string with default value.
 *
 * @signature template <typename THost>
 *            class ModifiedIterator<THost, ModPadding>;
 *
 * @tparam THost original iterator.
 */

/*!
 * @class ModPaddingString
 * @extends ModifiedString
 * @headerfile <seqan/modifier.h>
 * @brief Pad characters beyond the end of a string with default value.
 *
 * @signature template <typename THost>
 *            class ModifiedString<THost, ModPadding>;
 *
 * @tparam THost original string.
 */

template <typename THost>
struct ModPaddingCargo
{
    using TSize  = typename Size<THost>::Type;
    using TValue = typename Value<THost>::Type;

    TSize   _expandedSize   = 0;
    TSize   _remainingSteps = 0;
    TValue  _paddedValue    = TValue();
};

// ============================================================================
// Metafunctions
// ============================================================================

// --------------------------------------------------------------------------
// Metafunction Reference
// --------------------------------------------------------------------------

template <typename THost>
struct Reference<ModifiedString<THost, ModPadding> > :
    Reference<THost>
{};

template < typename THost>
struct Reference<ModifiedString<THost, ModPadding> const> :
    Reference<THost const>
{};

// --------------------------------------------------------------------------
// Metafunction Cargo                           [ModPadding ModifiedIterator]
// --------------------------------------------------------------------------

template <typename THost>
struct Cargo<ModifiedString<THost, ModPadding> >
{
    using Type = ModPaddingCargo<THost>;
};

template <typename THost>
struct Cargo<ModifiedIterator<THost, ModPadding> >
{
    using Type = ModPaddingCargo<typename Container<THost>::Type>;
};

// --------------------------------------------------------------------------
// Metafunction Iterator                          [ModPadding ModifiedString]
// --------------------------------------------------------------------------

template <typename THost>
struct Iterator<ModifiedString<THost, ModPadding>, Standard>
{
    typedef ModifiedIterator<typename Iterator<THost, Rooted>::Type, ModPadding> Type;
};

template <typename THost>
struct Iterator<ModifiedString<THost, ModPadding> const, Standard>
{
    typedef ModifiedIterator<typename Iterator<THost, Rooted>::Type, ModPadding> Type;
};

// --------------------------------------------------------------------------
// Metafunction DefaultIteratorSpec               [ModReverse ModifiedString]
// --------------------------------------------------------------------------

template <typename THost>
struct DefaultIteratorSpec< ModifiedString<THost, ModPadding> >
{
    typedef Rooted Type;
};

// --------------------------------------------------------------------------
// Metafunction AllowsFastRandomAccess
// --------------------------------------------------------------------------

template <typename THost>
struct AllowsFastRandomAccess<ModifiedString<THost, ModPadding> > :
    AllowsFastRandomAccess<THost>
{};

// ============================================================================
// Functions
// ============================================================================

// --------------------------------------------------------------------------
// Function expand()
// --------------------------------------------------------------------------

/*!
 * @fn ModPaddingString#expand
 * @headerfile <seqan/modifier.h>
 * @brief Expands the original string by the given size.
 *
 * @signature void expand(str, size[, pad])
 *
 * @param [in,out] str  The modified string to be padded.
 * @param [in]     size The number of padded characters.
 * @param [in]     pad  The character to pad the seequence with.
 * 
 * @datarace Not thread-safe.
 */

template <typename THost, typename TSize, typename TPadding>
inline void expand(ModifiedString<THost, ModPadding> & me,
                   TSize const newSize,
                   TPadding const & _padding)
{
    SEQAN_ASSERT(me._host != nullptr);

    cargo(me)._expandedSize = newSize;
    cargo(me)._paddedValue = _padding;
}

template <typename THost, typename TSize>
inline void expand(ModifiedString<THost, ModPadding> & me,
                   TSize const newSize)
{
    expand(me, newSize, typename Value<THost>::Type());
}

// --------------------------------------------------------------------------
// Function length()
// --------------------------------------------------------------------------

template <typename THost>
inline auto
length(ModifiedString<THost, ModPadding> const & me)
{
    return length(host(me)) + me._cargo._expandedSize;
}

// ----------------------------------------------------------------------------
// Function value()
// ----------------------------------------------------------------------------

template <typename THost, typename TPosition>
inline typename Reference<ModifiedString<THost, ModPadding> >::Type
value(ModifiedString<THost, ModPadding> & me, TPosition const pos)
{
    SEQAN_ASSERT_LT(pos, length(me));
    return (pos < length(host(me))) ? host(me)[pos] : cargo(me)._paddedValue;
}

template <typename THost, typename TPosition>
inline typename Reference<ModifiedString<THost, ModPadding> const>::Type
value(ModifiedString<THost, ModPadding> const & me, TPosition const pos)
{
    SEQAN_ASSERT_LT(pos, length(me));
    return (pos < length(host(me))) ? value(host(me), pos) : cargo(me)._paddedValue;
}

// --------------------------------------------------------------------------
// Function begin()                               [ModReverse ModifiedString]
// --------------------------------------------------------------------------

template < typename THost, typename TTagSpec>
inline typename Iterator< ModifiedString<THost, ModPadding> const >::Type
begin(ModifiedString<THost, ModPadding> const & me, Tag<TTagSpec> const & /*tag*/)
{
    typename Iterator<ModifiedString<THost, ModPadding> const, Standard>::Type temp_(begin(host(me), Rooted()));
    _copyCargo(temp_, me);
    cargo(temp_)._remainingSteps = cargo(me)._expandedSize;
    return temp_;
}

template < typename THost, typename TTagSpec>
inline typename Iterator< ModifiedString<THost, ModPadding> >::Type
begin(ModifiedString<THost, ModPadding> & me, Tag<TTagSpec> const & /*tag*/)
{
    typename Iterator<ModifiedString<THost, ModPadding>, Standard>::Type temp_(begin(host(me), Rooted()));

    _copyCargo(temp_, me);
    cargo(temp_)._remainingSteps = cargo(me)._expandedSize;
    return temp_;
}

// --------------------------------------------------------------------------
// Function end()                                 [ModReverse ModifiedString]
// --------------------------------------------------------------------------

template <typename THost, typename TTagSpec >
inline auto
end(ModifiedString<THost, ModPadding> const & me, Tag<TTagSpec> const)
{
    typename Iterator<ModifiedString<THost, ModPadding> const, Standard>::Type temp_(end(host(me), Rooted()));
    _copyCargo(temp_, me);
    cargo(temp_)._remainingSteps = 0;
    return temp_;
}

template <typename THost, typename TTagSpec >
inline auto
end(ModifiedString<THost, ModPadding> & me, Tag<TTagSpec> const)
{
    typename Iterator<ModifiedString<THost, ModPadding>, Standard>::Type temp_(end(host(me), Rooted()));
    _copyCargo(temp_, me);
    cargo(temp_)._remainingSteps = 0;
    return temp_;
}

// ----------------------------------------------------------------------------
// Function operator*()
// ----------------------------------------------------------------------------

template <typename THost>
inline auto
operator*(ModifiedIterator<THost, ModPadding> & me)
{
    return (atEnd(host(me))) ? cargo(me)._paddedValue : *host(me);
}

template <typename THost>
inline auto
operator*(ModifiedIterator<THost, ModPadding> const & me)
{
    return (atEnd(host(me))) ? (cargo(me)._paddedValue) : (*host(me));
}

// ----------------------------------------------------------------------------
// Function operator++()
// ----------------------------------------------------------------------------

template <typename THost>
inline ModifiedIterator<THost, ModPadding> &
operator++(ModifiedIterator<THost, ModPadding> & me)
{
    if (atEnd(host(me)))
        --cargo(me)._remainingSteps;
    else
        ++host(me);
    return me;
}

// ----------------------------------------------------------------------------
// Function operator+=()
// ----------------------------------------------------------------------------

template <typename THost, typename TSize>
inline ModifiedIterator<THost, ModPadding> &
operator+=(ModifiedIterator<THost, ModPadding> & me, TSize const steps)
{
    if (atEnd(host(me)))
    {
        cargo(me)._remainingSteps -= steps;   // Remove steps from remaining size.
    }
    else
    {
        auto rem = (end(container(host(me)), Rooted()) - host(me));
        if (static_cast<decltype(rem)>(steps) <= rem)  // Move host steps forward.
        {
            std::advance(host(me), steps);
        }
        else  // Move host rem forward and remove diff from cargo.
        {
            std::advance(host(me), rem);
            cargo(me)._remainingSteps -= (steps - rem);
        }
    }
    return me;
}

// ----------------------------------------------------------------------------
// Function operator--()
// ----------------------------------------------------------------------------

template <typename THost>
inline ModifiedIterator<THost, ModPadding> &
operator--(ModifiedIterator<THost, ModPadding> & me)
{
    if (cargo(me)._remainingSteps == cargo(me)._expandedSize)
        --host(me);
    else
        ++cargo(me)._remainingSteps;
    return me;
}

// ----------------------------------------------------------------------------
// Function operator-=()
// ----------------------------------------------------------------------------

template <typename THost, typename TSize>
inline ModifiedIterator<THost, ModPadding> &
operator-=(ModifiedIterator<THost, ModPadding> & me, TSize const steps)
{
    if (atEnd(host(me)))
    {
        auto rem = cargo(me)._expandedSize - cargo(me)._remainingSteps;
        if (steps <= rem)
        {
            cargo(me)._remainingSteps += steps;
        }
        else
        {
            cargo(me)._remainingSteps = cargo(me)._expandedSize;
            std::advance(host(me), -rem);
        }
    }
    else
    {
        std::advance(host(me), -steps);
    }
    return me;
}

// ----------------------------------------------------------------------------
// Function operator-()
// ----------------------------------------------------------------------------

template <typename THost>
inline typename Difference<ModifiedIterator<THost, ModPadding> >::Type
operator-(ModifiedIterator<THost, ModPadding> const & a,
          ModifiedIterator<THost, ModPadding> const & b)
{
    return host(a) - host(b) + cargo(b)._remainingSteps - cargo(a)._remainingSteps;
}

// --------------------------------------------------------------------------
// Function operator==()
// --------------------------------------------------------------------------

template <typename THost>
inline bool
operator == (ModifiedIterator<THost, ModPadding> const & a, ModifiedIterator<THost, ModPadding> const & b)
{
    return host(a) == host(b) && cargo(a)._remainingSteps == cargo(b)._remainingSteps;
}

// --------------------------------------------------------------------------
// Function operator!=()
// --------------------------------------------------------------------------

template <typename THost>
inline bool
operator != (ModifiedIterator<THost, ModPadding> const & a, ModifiedIterator<THost, ModPadding> const & b)
{
    return !(a == b);
}

}

#endif  // #ifndef INCLUDE_SEQAN_MODIFIER_MODIFIER_PADDING_H_
