#ifndef WIDGET_CSSBUILDER_H
#define WIDGET_CSSBUILDER_H


#include <string>
#include <vector>



namespace Widget {

class CSSBuilder
{
public:

    static CSSBuilder* instance();

    void initCSS();

    bool installCSS(const std::string &sourcePath, const std::string &cssName );

    std::vector<std::string> getCSSPathList() const;

private:
    CSSBuilder();

    std::vector<std::string> mFileListPath;

    static CSSBuilder* mBuilder;



};

} // namespace Widget

#endif // WIDGET_CSSBUILDER_H
