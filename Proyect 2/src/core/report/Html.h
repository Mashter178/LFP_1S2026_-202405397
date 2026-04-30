#ifndef HTML_H
#define HTML_H

#include <string>
#include <vector>

std::string htmlEscape(const std::string& input);
bool writeHtmlDocument(const std::string& outputPath, const std::string& title, const std::string& bodyHtml);
std::string buildMetricCard(const std::string& label, const std::string& value, const std::string& note = std::string());
std::string joinList(const std::vector<std::string>& values);

#endif