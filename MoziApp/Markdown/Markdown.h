#pragma once


namespace Markdown
{
	void LoadFonts(ImFont* H1, ImFont* H2 = NULL, ImFont* H3 = NULL);
	void MarkdownBegin(const std::string& markdown_, const std::filesystem::path& current_open_path);
}

