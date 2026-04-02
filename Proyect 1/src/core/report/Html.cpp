#include "Html.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <system_error>

std::string htmlEscape(const std::string& input) {
    std::string output;
    output.reserve(input.size());

    for (std::size_t i = 0; i < input.size(); ++i) {
        const char c = input[i];
        switch (c) {
            case '&': output += "&amp;"; break;
            case '<': output += "&lt;"; break;
            case '>': output += "&gt;"; break;
            case '"': output += "&quot;"; break;
            case '\'': output += "&#39;"; break;
            default: output += c; break;
        }
    }

    return output;
}

std::string joinList(const std::vector<std::string>& values) {
    std::ostringstream builder;
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i > 0) {
            builder << ", ";
        }
        builder << values[i];
    }
    return builder.str();
}

bool writeHtmlDocument(const std::string& outputPath, const std::string& title, const std::string& bodyHtml) {
    const std::filesystem::path path(outputPath);
    if (path.has_parent_path()) {
        std::error_code errorCode;
        std::filesystem::create_directories(path.parent_path(), errorCode);
    }

    std::ofstream out(outputPath.c_str(), std::ios::out | std::ios::trunc);
    if (!out) {
        return false;
    }

    out << "<!doctype html>\n";
    out << "<html lang=\"es\">\n";
    out << "<head>\n";
    out << "  <meta charset=\"utf-8\">\n";
    out << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    out << "  <title>" << htmlEscape(title) << "</title>\n";
    out << "  <style>\n";
    out << "    body { margin: 0; font-family: 'Segoe UI', Tahoma, sans-serif; background: linear-gradient(180deg, #f7f9fc 0%, #eef3f9 100%); color: #1f2937; }\n";
    out << "    .page { max-width: 1280px; margin: 0 auto; padding: 28px 20px 40px; }\n";
    out << "    .hero { background: linear-gradient(135deg, #0f172a 0%, #1d4ed8 100%); color: #fff; border-radius: 20px; padding: 24px; box-shadow: 0 18px 40px rgba(15, 23, 42, 0.18); margin-bottom: 20px; }\n";
    out << "    .hero h1 { margin: 0 0 8px 0; font-size: 30px; }\n";
    out << "    .hero p { margin: 0; opacity: 0.92; }\n";
    out << "    .grid { display: grid; gap: 16px; }\n";
    out << "    .grid.cards { grid-template-columns: repeat(auto-fit, minmax(180px, 1fr)); }\n";
    out << "    .card { background: #fff; border: 1px solid #d9e2ee; border-radius: 16px; padding: 16px; box-shadow: 0 10px 28px rgba(15, 23, 42, 0.05); }\n";
    out << "    .card h2, .card h3 { margin: 0 0 12px 0; }\n";
    out << "    .metric { font-size: 28px; font-weight: 700; margin: 6px 0 0; }\n";
    out << "    .label { text-transform: uppercase; letter-spacing: 0.08em; font-size: 12px; color: #64748b; }\n";
    out << "    .muted { color: #64748b; }\n";
    out << "    .ok { color: #065f46; font-weight: 600; }\n";
    out << "    .warn { color: #b45309; font-weight: 600; }\n";
    out << "    .danger { color: #b91c1c; font-weight: 600; }\n";
    out << "    table { width: 100%; border-collapse: collapse; font-size: 14px; overflow: hidden; }\n";
    out << "    th, td { border-bottom: 1px solid #d9e2ee; padding: 10px 12px; text-align: left; vertical-align: top; }\n";
    out << "    th { background: #eff6ff; color: #0f172a; }\n";
    out << "    tr:nth-child(even) td { background: #fbfdff; }\n";
    out << "    .section { margin-top: 18px; }\n";
    out << "    .section-title { margin: 0 0 10px 0; font-size: 18px; }\n";
    out << "    .pill { display: inline-block; padding: 4px 10px; border-radius: 999px; background: #e2e8f0; color: #0f172a; font-size: 12px; font-weight: 600; }\n";
    out << "    .pill.ok { background: #dcfce7; color: #166534; }\n";
    out << "    .pill.warn { background: #fef3c7; color: #92400e; }\n";
    out << "    .pill.danger { background: #fee2e2; color: #991b1b; }\n";
    out << "    .spacer { height: 8px; }\n";
    out << "    .small { font-size: 13px; }\n";
    out << "    .stack > * + * { margin-top: 10px; }\n";
    out << "    .mono { font-family: Consolas, 'Courier New', monospace; }\n";
    out << "    .patient-block { border: 1px solid #d9e2ee; border-radius: 16px; padding: 16px; background: #fff; }\n";
    out << "    .patient-block + .patient-block { margin-top: 14px; }\n";
    out << "    .subtle { background: #f8fafc; border-radius: 12px; padding: 12px; border: 1px solid #e2e8f0; }\n";
    out << "  </style>\n";
    out << "</head>\n";
    out << "<body>\n";
    out << "  <div class=\"page\">\n";
    out << "    <div class=\"hero\">\n";
    out << "      <h1>" << htmlEscape(title) << "</h1>\n";
    out << "      <p>MedLang - reportes generados desde la estructura del hospital cargada en memoria.</p>\n";
    out << "    </div>\n";
    out << bodyHtml;
    out << "  </div>\n";
    out << "</body>\n";
    out << "</html>\n";

    return true;
}

std::string buildMetricCard(const std::string& label, const std::string& value, const std::string& note) {
    std::ostringstream html;
    html << "<div class=\"card\">";
    html << "<div class=\"label\">" << htmlEscape(label) << "</div>";
    html << "<div class=\"metric\">" << htmlEscape(value) << "</div>";
    if (!note.empty()) {
        html << "<div class=\"small muted\">" << htmlEscape(note) << "</div>";
    }
    html << "</div>";
    return html.str();
}