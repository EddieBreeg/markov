#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "Markov.hpp"
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

namespace {
	std::string_view ParseLine(std::string_view& text)
	{
		if (text.empty())
			return {};

		size_t pos = text.find_first_of("\r\n");
		std::string_view line = line = text.substr(0, pos);

		while (pos < text.length() && (text[pos] == '\r' || text[pos] == '\n'))
			++pos;

		if (pos > text.length())
			text = {};
		else
			text.remove_prefix(pos);

		return line;
	}

	std::string_view CsvNext(std::string_view& line)
	{
		const size_t pos = std::min(line.find(','), line.length());
		const std::string_view val = line.substr(0, pos);
		line.remove_prefix(pos);
		return val;
	}

	std::vector<char> ReadFile(const char* path)
	{
		std::ifstream file{ path, std::ios::binary };
		if (!file.is_open())
		{
			char err[1024];
			strerror_s(err, errno);
			std::cerr << err << '\n';
			return {};
		}
		file.seekg(0, std::ios::end);
		std::vector<char> data(file.tellg());
		file.seekg(0);
		file.read(data.data(), data.size());
		return data;
	}

	void GenerateName(
		mrkv::MarkovModel& model,
		std::ostream& output,
		mrkv::ShiftingArray<std::string_view> tokenSequence,
		std::mt19937_64& rng,
		const uint32_t minSize)
	{
		tokenSequence.m_Size = 0;
		std::string_view next = model.ChooseNext(tokenSequence, rng);

		uint32_t size = 0;

		while (next.size() || size < minSize)
		{
			output << next;
			if (next.size())
				++size;

			tokenSequence.AddEmplace(next);
			next = model.ChooseNext(tokenSequence, rng);
		}
		output << '\n';
	}
} // namespace

int main(int argc, char const* argv[])
{
	if (argc < 2)
		return 1;

	uint32_t minSize = argc >= 3 ? atoi(argv[2]) : 0;

	static constexpr uint32_t order = 3;

	const std::vector data = ReadFile(argv[1]);

	std::string_view text = { data.data(), data.size() };
	mrkv::MarkovModel model;

	while (text.length())
	{
		std::string_view line = ParseLine(text);
		std::string_view firstName = CsvNext(line);
		model.ConsumeText(firstName, mrkv::Utf8Lexer, order);
		model.ConsumeText(firstName, mrkv::Utf8Lexer, order - 1);
	}

	std::random_device rd;
	const uint64_t seed = (uint64_t(rd()) << 32) | rd();
	std::mt19937_64 rng{ seed };

	std::string_view buf[order];
	mrkv::ShiftingArray<std::string_view> sequence{ buf, order };

	for (int i = 0; i < 10; i++)
	{
		GenerateName(model, std::cout, sequence, rng, minSize);
	}
}
