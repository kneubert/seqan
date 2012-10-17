// ==========================================================================
//                 seqan - the library for sequence analysis
// ==========================================================================
// Copyright (c) 2006-2011, Knut Reinert, FU Berlin
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
// Author: Jochen Singer <jochen.singer@fu-berlin.de>
// ==========================================================================

#ifndef INDEX_FM_PREFIX_SUM_TABLE_H_
#define INDEX_FM_PREFIX_SUM_TABLE_H_

namespace seqan {

// ==========================================================================
// Forwards
// ==========================================================================

template <typename TChar, typename TSpec>
class PrefixSumTable;

/**
.Tag.PrefixSumTable Fibres
..summary:Tag to select a specific fibre of a @Class.CompressedSA@.
..remarks:These tags can be used to get @Metafunction.Fibre.Fibres@ of a sparse string.
..cat:Index

..tag.FibreEntries:The entries in the prefix sum table.

..see:Metafunction.Fibre
..see:Function.getFibre
..include:seqan/index.h
*/
struct FibreEntries_;
typedef Tag<FibreEntries_> const FibreEntries;

// ==========================================================================
// Metafunctions
// ==========================================================================

template <typename TChar, typename TSpec>
struct GetValue<PrefixSumTable<TChar, TSpec> >
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type TEntries;
    typedef typename Value<TEntries>::Type Type;
};

template <typename TChar, typename TSpec>
struct GetValue<PrefixSumTable<TChar, TSpec> const>
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec> const, FibreEntries>::Type TEntries;
    typedef typename Value<TEntries>::Type Type;
};

// ==========================================================================
template <typename TChar, typename TSpec>
struct Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>
{
    typedef String<unsigned> Type;
};

template <typename TChar, typename TSpec>
struct Fibre<PrefixSumTable<TChar, TSpec> const, FibreEntries>
{
    typedef String<unsigned> const Type;
};

// ==========================================================================
template <typename TChar, typename TSpec>
struct Size<PrefixSumTable<TChar, TSpec> >
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type TEntries;
    typedef typename Size<TEntries>::Type Type;
};

// ==========================================================================
template <typename TChar, typename TSpec>
struct Infix<PrefixSumTable<TChar, TSpec> >
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type TEntries;
    typedef typename Infix<TEntries>::Type Type;
};

template <typename TChar, typename TSpec>
struct Infix<PrefixSumTable<TChar, TSpec> const>
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec> const, FibreEntries>::Type TEntries;
    typedef typename Infix<TEntries>::Type Type;
};

// ==========================================================================
template <typename TSpec>
struct CharacterValue;

template <typename TChar, typename TSpec>
struct CharacterValue<PrefixSumTable<TChar, TSpec> >
{
    typedef typename MakeUnsigned<TChar>::Type TUChar;
    typedef TUChar Type;
};

template <typename TChar, typename TSpec>
struct CharacterValue<PrefixSumTable<TChar, TSpec> const>
{
    typedef typename MakeUnsigned<TChar>::Type TUChar;
    typedef TUChar Type;
};

// ==========================================================================
template <typename TChar, typename TSpec>
struct Reference<PrefixSumTable<TChar, TSpec> >
{
    typedef typename Value<PrefixSumTable<TChar, TSpec> >::Type & Type;
};

template <typename TChar, typename TSpec>
struct Reference<PrefixSumTable<TChar, TSpec> const>
{
    typedef typename Value<PrefixSumTable<TChar, TSpec> >::Type const & Type;
};

// ==========================================================================
template <typename TChar, typename TSpec>
struct Value<PrefixSumTable<TChar, TSpec> >
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type TEntries;
    typedef typename Value<TEntries>::Type Type;
};

template <typename TChar, typename TSpec>
struct Value<PrefixSumTable<TChar, TSpec> const>
{
    typedef typename Fibre<PrefixSumTable<TChar, TSpec> const, FibreEntries>::Type TEntries;
    typedef typename Value<TEntries>::Type const Type;
};

// ==========================================================================
// Classes
// ==========================================================================
/**
.Class.PrefixSumTable:
..cat:Index
..summary:PrefixSumTable is a data structure storing for each character x of 
the alphabet how many smaller than x are in a given text.
..signature:PrefixSumTable<TChar, TSpec>
..param.TChar:The character type
..param.TSpec:A specialisation tag.
...default:void
..include:seqan/Index.h
*/
template <typename TChar, typename TSpec = void>
class PrefixSumTable
{
    typedef typename Fibre<PrefixSumTable, FibreEntries>::Type TEntries;
    typedef typename Value<TEntries>::Type TEntry;

public:
    TEntries entries;

    PrefixSumTable() :
        entries()
    {}

    PrefixSumTable(String<TChar> const & text) :
        entries()
    {
        createPrefixSumTable(*this, text);
    }

    template <typename TPos>
    inline TEntry & operator[](TPos pos)
    {
        return value(*this, pos);
    }

    template <typename TPos>
    inline TEntry const & operator[](TPos pos) const
    {
        return value(*this, pos);
    }

    inline bool operator==(PrefixSumTable const & other)
    {
        return entries == other.entries;
    }

    inline bool operator==(PrefixSumTable const & other) const
    {
        return entries == other.entries;
    }
};

// ==========================================================================
// Functions
// ==========================================================================
///.Function.clear.param.object.type:Class.PrefixSumTable
///.Function.clear.class:Class.PrefixSumTable
template <typename TChar, typename TSpec>
inline void clear(PrefixSumTable<TChar, TSpec> & prefixSumTable)
{
    clear(prefixSumTable.entries);
}

// ==========================================================================
/**
.Function.createPrefixSumTable
..summary:Creates the prefix sum table
..signature:createPrefixSumTable(prefixSumTable, text)
..param.prefixSumTable:The prefix sum table to be constructed.
...type:Class.PrefixSumTable.
..param.text:The underlying text.
...type.class:String
...type.class:StringSet
..include:seqan/index.h
*/

template <typename TChar, typename TSpec, typename TText>
inline void createPrefixSumTable(PrefixSumTable<TChar, TSpec> & prefixSumTable, TText const & text)
{
    typedef PrefixSumTable<TChar, TSpec> TPrefixSumTable;
    typedef typename Value<typename Fibre<TPrefixSumTable, FibreEntries>::Type>::Type TPrefixSumValue;

    TPrefixSumTable freq;
    _getFrequencies(freq, text);

    unsigned alpSize = length(freq);
    resize(prefixSumTable, alpSize + 1, 0);


    TPrefixSumValue temp = 0;
    TPrefixSumValue sum = 0;
    for (TPrefixSumValue i = 0; i < alpSize; ++i)
    {
        temp = getPrefixSum(freq, i);
        setPrefixSum(prefixSumTable, sum, i);
        sum += temp;
    }
    setPrefixSum(prefixSumTable, sum, alpSize);
}

// ==========================================================================
/**
.Function.getAlphabetSize
..summary:Returns the number of different characters in the prefix sum table.
..signature:getAlphabetSize(prefixSumTable)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable.
..include:seqan/index.h
*/
template <typename TChar, typename TSpec>
unsigned getAlphabetSize(PrefixSumTable<TChar, TSpec> const & pst)
{
    return length(pst.entries) - 1;
}

// ==========================================================================
/**
.Function.getCharacterPosition
..summary:Returns the position of a given character within the prefix sum table.
..signature:getCharacterPosition(prefixSumTable, character)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable.
..param.character:A character.
..include:seqan/index.h
*/
template <typename TDummy, typename TChar>
inline unsigned getCharacterPosition(TDummy const & /*tag*/, TChar character)
{
    return ordValue(character);
}

// ==========================================================================
template <typename TChar, typename TSpec, typename TChar2>
inline unsigned getCharacterPosition(PrefixSumTable<TChar, TSpec> const & /*tag*/, TChar2 character)
{
    return ordValue(character);
}

// ==========================================================================
/**
.Function.PrefixSumTable#getCharacter
..summary:Returns the character of a given position within the prefix sum table.
..signature:getCharacter(prefixSumTable, pos)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable.
..param.pos:A position.
...type:Concept.UnsignedIntegerConcept
..include:seqan/index.h
*/
template <typename TChar, typename TSpec, typename TPos>
inline typename CharacterValue<PrefixSumTable<TChar, TSpec> const>::Type
getCharacter(PrefixSumTable<TChar, TSpec> const & /*tag*/, TPos const pos)
{
    return static_cast<TChar>(pos);
}

// ==========================================================================
template <typename TChar, typename TSpec, typename TBeginPos, typename TEndPos>
unsigned _getPivotPosition(PrefixSumTable<TChar, TSpec> const & pst, TBeginPos beginPos, TEndPos endPos)
{
    TBeginPos realBeginPos = beginPos + 1;
    TEndPos realEndPos = endPos + 1;
    unsigned lengthRange = realEndPos - realBeginPos + 1;
    unsigned pivotPos = realBeginPos + lengthRange / 2 - 1;

    unsigned tooSmallValues = pst[beginPos];

    int direction;
    ((pst[pivotPos] - tooSmallValues) >= (pst[realEndPos] - pst[pivotPos])) ? direction = -1 : direction = 1;
    long currentMin = pst[realEndPos] + 1;

    if (direction == -1)
    {
        while ((pivotPos >= realBeginPos) && std::abs((long)(pst[pivotPos] - tooSmallValues) - (long)((pst[realEndPos] - pst[pivotPos]))) <= currentMin)
        {
            currentMin = std::abs((long)((pst[pivotPos] - tooSmallValues)) - (long)((pst[realEndPos] - pst[pivotPos])));
            --pivotPos;
        }
        ++pivotPos;
    }
    else
    {
        while (std::abs((long)((pst[pivotPos] - tooSmallValues)) - (long)((pst[realEndPos] - pst[pivotPos]))) < currentMin && (pivotPos < realEndPos))
        {
            currentMin = std::abs((long)((pst[pivotPos] - tooSmallValues)) - (long)((pst[realEndPos] - pst[pivotPos])));
            ++pivotPos;
        }
        --pivotPos;
    }

    return pivotPos;
}

// ==========================================================================
/**
.Function.getPrefixSum
..summary:Returns the prefix sum of a given position. 
..signature:getPrefixSum(prefixSumTable, pos)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable
..param.pos:A position.
...type:Concept.UnsignedIntegerConcept
..include:seqan/index.h
*/
template <typename TChar, typename TSpec, typename TPos>
typename Value<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type
getPrefixSum(PrefixSumTable<TChar, TSpec> const & pst, TPos const pos)
{
    return getValue(pst, pos);
}

// ==========================================================================
template <typename TChar, typename TSpec, typename TPos>
inline typename GetValue<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type
getValue(PrefixSumTable<TChar, TSpec> & pst, TPos const pos)
{
    return pst.entries[pos];
}

template <typename TChar, typename TSpec, typename TPos>
inline typename GetValue<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type const
getValue(PrefixSumTable<TChar, TSpec> const & pst, TPos const pos)
{
    return pst.entries[pos];
}

// ==========================================================================
/**
.Function.PrefixSumTable#getFibre:
..summary:Returns a specific fibre of a container.
..signature:getFibre(container, fibreTag)
..class:Class.PrefixSumTable
..cat:Index
..param.container:The container holding the fibre.
...type:Class.PrefixSumTable
..param.fibreTag:A tag that identifies the @Metafunction.Fibre@.
...type:Tag.PrefixSumTable Fibres
..returns:A reference to the @Metafunction.Fibre@ object.
..include:seqan/index.h
..example.code:
Index< String<char> > index_esa("tobeornottobe");

String<char> & text = getFibre(indexEsa, EsaText());
*/
template <typename TChar, typename TSpec>
inline typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type const &
getFibre(PrefixSumTable<TChar, TSpec> const & pst, FibreEntries const /*tag*/)
{
    return pst.entries;
}

template <typename TChar, typename TSpec>
inline typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type &
getFibre(PrefixSumTable<TChar, TSpec> & pst, FibreEntries const /*tag*/)
{
    return pst.entries;
}

// ==========================================================================
template <typename TChar, typename TSpec, typename TNumDollar>
void _insertDollar(PrefixSumTable<TChar, TSpec> & pst, TNumDollar const numDollar)
{
    for (unsigned i = 0; i < length(pst); ++i)
        prefixSum(pst, i) = getPrefixSum(pst, i) + numDollar;
}

// ==========================================================================
///.Function.length.param.type:Class.PrefixSumTable
template <typename TChar, typename TSpec>
inline typename Size<PrefixSumTable<TChar, TSpec> >::Type
length(PrefixSumTable<TChar, TSpec> const & pst)
{
    return length(pst.entries);
}

// ==========================================================================
/**
.Function.prefixSum
..summary:Returns a reference to the entry of the prefix sum table of a given position. 
..signature:prefixSum(prefixSumTable, pos)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable
..param.pos:A position.
...type:Concept.UnsignedIntegerConcept
..include:seqan/index.h
*/
template <typename TChar, typename TSpec, typename TPos>
typename Value<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type &
prefixSum(PrefixSumTable<TChar, TSpec>&pst, TPos const pos)
{
    return value(pst, pos);
}

template <typename TChar, typename TSpec, typename TPos>
typename Value<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type const &
prefixSum(PrefixSumTable<TChar, TSpec> const & pst, TPos const pos)
{
    return value(pst, pos);
}

// ==========================================================================
///.Function.resize.param.type:Class.PrefixSumTable
template <typename TChar, typename TSpec, typename TSize>
inline void resize(PrefixSumTable<TChar, TSpec> & pst, TSize size)
{
    resize(pst.entries, size);
}

template <typename TChar, typename TSpec, typename TSize, typename TValue>
inline void resize(PrefixSumTable<TChar, TSpec> & pst, TSize size, TValue value)
{
    resize(pst.entries, size, value);
}

// ==========================================================================
/**
.Function.setPrefixSum
..summary:Returns a reference to the entry of the prefix sum table of a given position. 
..signature:setPrefixSum(prefixSumTable, value, pos)
..param.prefixSumTable:A prefix sum table.
...type:Class.PrefixSumTable
..param.value:A specified value to be inserted.
..param.pos:A position.
...type:Concept.UnsignedIntegerConcept
..include:seqan/index.h
*/

template <typename TChar, typename TSpec, typename TValue, typename TPos>
inline void setPrefixSum(PrefixSumTable<TChar, TSpec> & pst, TValue value, TPos const pos)
{
    pst.entries[pos] = value;
}

// ==========================================================================
///.Function.value.param.type:Class.PrefixSumTable
template <typename TChar, typename TSpec, typename TPos>
inline typename Value<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type &
value(PrefixSumTable<TChar, TSpec>&pst, TPos const pos)
{
    return pst.entries[pos];
}

template <typename TChar, typename TSpec, typename TPos>
inline typename Value<typename Fibre<PrefixSumTable<TChar, TSpec>, FibreEntries>::Type>::Type const &
value(PrefixSumTable<TChar, TSpec> const & pst, TPos const pos)
{
    return pst.entries[pos];
}

// ==========================================================================
/**
.Function.open
..param.object:
...type:Class.LfTable
*/
template <typename TChar, typename TSpec>
inline bool open(
    PrefixSumTable<TChar, TSpec> & pst,
    const char * fileName,
    int openMode)
{
    String<char> name;
    name = fileName;    append(name, ".pst");
    if (!open(getFibre(pst, FibreEntries()), toCString(name), openMode))
    {
        return false;
    }
    return true;

}

template <typename TChar, typename TSpec>
inline bool open(
    PrefixSumTable<TChar, TSpec> & pst,
    const char * fileName)
{
    return open(pst, fileName, DefaultOpenMode<PrefixSumTable<TChar, TSpec> >::VALUE);
}

/**
.Function.save
..param.object:
...type:Class.LfTable
*/
template <typename TChar, typename TSpec>
inline bool save(
    PrefixSumTable<TChar, TSpec> const & pst,
    const char * fileName,
    int openMode)
{
    String<char> name;
    name = fileName;    append(name, ".pst");
    if (!save(getFibre(pst, FibreEntries()), toCString(name), openMode))
    {
        return false;
    }
    return true;
}

template <typename TChar, typename TSpec>
inline bool save(
    PrefixSumTable<TChar, TSpec> const & pst,
    const char * fileName)
{
    return save(pst, fileName, DefaultOpenMode<PrefixSumTable<TChar, TSpec> >::VALUE);
}

}
#endif // SANDBOX_MY_SANDBOX_APPS_FMINDEX_PREFIX_SUM_TABLE_H_