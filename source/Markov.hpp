#pragma once

#include <cwctype>
#include <new>
#include <random>
#include <string_view>
#include <unordered_map>

namespace mrkv {
	using Lexer = std::string_view(std::string_view text);

	// Default lexer returns the next character from the stream, or an empty view if text
	// is empty
	[[nodiscard]] inline std::string_view DefaultLexer(std::string_view text)
	{
		if (text.empty())
			return {};

		return text.substr(0, 1);
	}

	[[nodiscard]] inline std::string_view WordLexer(std::string_view text)
	{
		size_t pos = 0;
		while (pos < text.length() && std::iswspace(text[pos]))
			++pos;

		if (pos == text.length())
			return {};

		text.remove_prefix(pos);
		return text.substr(0, text.find_first_of(" \t\r\n"));
	}

	[[nodiscard]] inline std::string_view Utf8Lexer(std::string_view text)
	{
		if (text.empty())
			return {};

		const uint8_t byte = text[0];
		uint32_t len = 0;
		if (!(byte & 0x80))
			len = 1;
		else if ((byte & 0xE0) == 0xC0)
			len = 2;
		else if ((byte & 0xF0) == 0xE0)
			len = 3;
		else if ((byte & 0xF8) == 0xF0)
			len = 4;

		if (len > text.length())
			return {};

		return text.substr(0, len);
	}

	template <class T>
	struct ShiftingArray
	{
		T* m_Buffer = nullptr;
		size_t m_Capacity = 0;
		size_t m_Size = 0;
		size_t m_Pos = 0;

		template <class... Args>
		void AddEmplace(Args&&... args)
		{
			if (m_Size < m_Capacity)
				++m_Size;

			new (m_Buffer + m_Pos) T{ std::forward<Args>(args)... };
			if (++m_Pos == m_Capacity)
				m_Pos = 0;
		}

		const T& operator[](int64_t index) const
		{
			return m_Buffer[(m_Pos + index + m_Size) % m_Size];
		}
	};

	class MarkovModel
	{
	public:
		MarkovModel() = default;
		MarkovModel(
			std::string_view text,
			Lexer* lexer = &DefaultLexer,
			uint32_t order = 1);
		~MarkovModel() = default;

		void ConsumeText(
			std::string_view text,
			Lexer* lexer = &DefaultLexer,
			uint32_t order = 1);

		struct SeqStats
		{
			uint64_t m_TotalCount = 0;
			std::unordered_map<std::string_view, uint64_t> m_Frequencies;
		};

		const std::unordered_map<uint32_t, SeqStats>& GetData() const noexcept
		{
			return m_Data;
		}

		template <class Rng>
		std::string_view ChooseNext(ShiftingArray<std::string_view> sequence, Rng&& g)
			const
		{
			std::string_view result;

			const auto it = m_Data.find(HashSequence(sequence));
			if (it == m_Data.end())
				return result;

			std::uniform_int_distribution<uint64_t> dist{
				0,
				it->second.m_TotalCount - 1,
			};
			const uint64_t r = dist(g);
			uint64_t sum = 0;

			for (auto&& [next, freq] : it->second.m_Frequencies)
			{
				if ((sum += freq) > r)
				{
					result = next;
					break;
				}
			}

			return result;
		}

	private:
		void Record(uint32_t hash, std::string_view next);

		static uint32_t HashSequence(
			const mrkv::ShiftingArray<std::string_view>& sequence);

		std::unordered_map<uint32_t, SeqStats> m_Data;
	};
} // namespace mrkv
