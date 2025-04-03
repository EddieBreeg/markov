#include "Alloca.hpp"
#include <Markov.hpp>

namespace {
	std::string_view RemoveSub(std::string_view text, std::string_view sub)
	{
		return text.substr(sub.data() + sub.size() - text.data());
	}

	uint32_t HashString(std::string_view str, uint32_t hash = 0)
	{
		for (const char c : str)
		{
			hash += c;
			hash += hash << 10;
			hash ^= hash >> 6;
		}
		hash *= 0x68f96d6d;
		hash ^= hash >> 7;
		hash *= hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;
		return hash;
	}
} // namespace

namespace mrkv {
	MarkovModel::MarkovModel(std::string_view text, Lexer* lexer, uint32_t order)
	{
		ConsumeText(text, lexer, order);
	}

	void MarkovModel::ConsumeText(std::string_view text, Lexer* lexer, uint32_t order)
	{
		std::string_view* buf = Alloca(std::string_view, order);
		ShiftingArray<std::string_view> sequence{ buf, order };
		std::string_view next = lexer(text);

		uint32_t hash = 0;
		size_t n = 0;

		while (next.length())
		{
			++n;
			Record(hash, next);
			text = RemoveSub(text, next);

			sequence.AddEmplace(next);
			hash = n > order ? HashSequence(sequence) : HashString(next, hash);
			next = lexer(text);
		}

		Record(hash, "");
	}

	void MarkovModel::Record(uint32_t hash, std::string_view next)
	{
		SeqStats& stats = m_Data[hash];
		++stats.m_TotalCount;
		++stats.m_Frequencies[next];
	}

	uint32_t MarkovModel::HashSequence(
		const mrkv::ShiftingArray<std::string_view>& sequence)
	{
		if (!sequence.m_Size)
			return 0;
		uint32_t hash = 0;
		for (size_t i = 0; i < sequence.m_Size; ++i)
		{
			hash = HashString(sequence[i], hash);
		}
		return hash;
	}
} // namespace mrkv
