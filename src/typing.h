#include "token.h"

#include <filesystem>

namespace black
{
	enum class StaticTypingCode
	{
		Ok, Error
	};

	class StaticTypingResult
	{
	public:
		StaticTypingResult(StaticTypingCode code, const std::string& message)
			: m_Code(code), m_Message(message) {}
		virtual ~StaticTypingResult() = default;

		inline StaticTypingCode GetCode() const { return m_Code; }
		inline const std::string& GetMessage() const { return m_Message; }

		inline bool IsOk() const { return m_Code == StaticTypingCode::Ok; }

		inline static StaticTypingResult Ok(const std::string& message)
		{
			return StaticTypingResult(StaticTypingCode::Ok, message);
		}

		inline static StaticTypingResult Error(const std::string& message)
		{
			return StaticTypingResult(StaticTypingCode::Error, message);
		}
	private:
		StaticTypingCode m_Code;
		std::string m_Message;
	};

	StaticTypingResult st_validate(const std::vector<Op>& code, const std::filesystem::path& path);
}
