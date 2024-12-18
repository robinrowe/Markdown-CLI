// markdown.cpp

#include <string>
#include <regex>
#include <list>  
#include "markdown.h"

struct token {
    std::string rex;    // Regex to find the markdown token
    int length1;        // Length of the first token ("**" = 2, "*" = 1)
    int length2;        // Length of the last token (quotes only have at the beginning, newlines at the end, etc)
    std::string tag;    // Equivalent tag
};

std::string markdown(std::string markdown){
    std::smatch match;  // Match object (stores match information)

    ////////////////////////////// HEADERS //////////////////////////////////////
    std::regex headers("#+[^\\r\\n]+(\\r|\\n|\\r\\n)"); // Regex for the headers

    while (std::regex_search (markdown, match, headers)) {

        auto match_begin = match.position();
        auto match_end   = match_begin + match.length();

        printf("Processing match: %s", markdown.substr(match_begin, match.length()).c_str());

        int j;
        for(j = 0; markdown[j+match_begin]=='#'; j++){}     // So count the number of '#' in a row,


        markdown.insert(match_end, "</h" + std::to_string(j) + "><hr/>");
        markdown.replace(match_begin, j, "<h" + std::to_string(j) + ">");
    }
    
    //////////////////////////////// LINKS //////////////////////////////////////
    std::regex links("\\[[^\\n\\r]*\\]\\([^\\n\\r]*\\)"); // Regex for the headers
    std::regex linkHref("\\([^\\n\\r]*\\)");

    std::smatch match2;  // Match object (stores match information)
    while (std::regex_search (markdown, match, links)) {

        ptrdiff_t match_begin = match.position();
        ptrdiff_t match_end   = match_begin + match.length();
        
        std::string text = "";
        std::string link = "";

        ptrdiff_t i, count;
        for(i = match_begin+1, count = 1; count > 0; i++){
            char c = markdown[i];

            if(c == '[') count++;
            else if(c == ']') count--;

            if(count > 0) text += c;
        }

        for(i += 1, count = 1; count > 0; i++){
            char c = markdown[i];

            if(c == '(') count++;
            else if(c == ')') count--;

            if(count > 0) link += c;
        }

        std::string finalLink = "<a href=\"" + link + "\">" + text + "</a>";

        printf("Processing link: %s", finalLink.c_str());
        markdown.replace(match_begin, match.length(), finalLink);
    }

    ////////////////////////// OTHER TOKENS ////////////////////////////////////
    //token{"[^\\r\\n]+((\\r|\\n|\\r\\n)[^\\r\\n]+)*",0,2,  "p"}       // Paragraphs regex

    std::list<token> tokens = {
        token{"`[^`\\n]+`",                             1,1,  "code"},   // Code tag regex
        token{"(\\*){2}[^(\\*){2}\\r\\n]+(\\*){2}",     2,2,  "b"},      // Bold tag regex    (**)
        token{"(_){2}[^(_){2}\\r\\n]+(_){2}",           2,2,  "b"},      // Bold tag regex    (__)
        token{"\\*[^\\*\\r\\n]+\\*",                    1,1,  "i"},      // Italic tag regex  (*)
        token{"_[^\\*\\r\\n]+_",                        1,1,  "i"}      // Italic tag regex  (_)
    };

    for(token tok: tokens){
        printf("Checking for %s\n", tok.tag.c_str());
        std::regex tmp(tok.rex);
        
        ptrdiff_t match_end, match_begin;
        while (std::regex_search (markdown, match, tmp)) {
            match_begin = match.position();
            match_end   = match_begin + match.length();

            printf("Processing match: %s\n", markdown.substr(match_begin, match.length()).c_str());

            markdown.replace(match_end-tok.length2, tok.length2, "</" + tok.tag + ">");
            markdown.replace(match_begin, tok.length1, "<" + tok.tag + ">");
        }

        printf("Done checking %s\n", tok.tag.c_str());
    }

    ////////////////////////////// LINE BREAKS ///////////////////////////////////
    for(int i = 0; i<markdown.length(); i++){
        if(markdown[i] == '\n'){
            markdown.insert(i, "<br/>");
            i+=6;
        }
    }

    return markdown;
}