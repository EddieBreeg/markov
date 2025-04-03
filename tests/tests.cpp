#include <Markov.hpp>
#include <cassert>

int main(int argc, char const* argv[])
{
	{
		std::string_view str = "foobar baz";
		assert(mrkv::DefaultLexer(str) == str.substr(0, 1));
		assert(mrkv::WordLexer(str) == str.substr(0, 6));

		str.remove_prefix(6);
		assert(mrkv::WordLexer(str) == str.substr(1, 3));
	}
	{
		std::string_view str = "¢";
		assert(mrkv::Utf8Lexer(str) == "¢");
	}
	{
		int buffer[4] = {};
		mrkv::ShiftingArray<int> seq{ buffer, 4 };

		seq.AddEmplace(1);
		assert(seq[0] == 1);

		seq.AddEmplace(2);
		assert(seq[0] == 1);
		assert(seq[1] == 2);

		seq.AddEmplace(3);
		assert(seq[0] == 1);
		assert(seq[1] == 2);
		assert(seq[2] == 3);

		seq.AddEmplace(4);
		assert(seq[0] == 1);
		assert(seq[1] == 2);
		assert(seq[2] == 3);
		assert(seq[3] == 4);

		seq.AddEmplace(5);
		assert(seq[0] == 2);
		assert(seq[1] == 3);
		assert(seq[2] == 4);
		assert(seq[3] == 5);
	}
	return 0;
}
