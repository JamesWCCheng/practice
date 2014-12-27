#pragma once

class UnicodeBlock
{
public:
	UnicodeBlock(unsigned short low, unsigned short high)
	{
		Low = low;
		High = high;
	}
	// Chinese
	static  UnicodeBlock CJK_RADICALS_SUPPLEMENT;
	static  UnicodeBlock KANGXI_RADICALS;
	static  UnicodeBlock BOPOMOFO;
	static  UnicodeBlock CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A;
	static  UnicodeBlock CJK_UNIFIED_IDEOGRAPHS;
	static  UnicodeBlock CJK_COMPATIBILITY_IDEOGRAPHS;


	// Japanese
	static  UnicodeBlock HIRAGANA;
	static  UnicodeBlock KATAKANA;
	static  UnicodeBlock KATAKANA_PHONETIC_EXTENSIONS;

	// Korean
	static  UnicodeBlock HANGUL_JAMO;
	static  UnicodeBlock HANGUL_COMPATIBILITY_JAMO;
	static  UnicodeBlock HANGUL_SYLLABLES;


	unsigned short Low;
	unsigned short High;

	

	bool Contain(unsigned short code)
	{
		return (Low <= code && code <= High);
	}
};
