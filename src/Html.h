// Html.h

#ifndef HTML_h
#define HTML_h

#include <string.h>
#include <list>
#include "Component.h"

class Html: public Component{
    private:
        Component *head, *body;
    
    public:
        Html();                             // Default constructor (no option to set the text of the component), adds the "head" and "body" tags.
                                            // All the components will be added to the "body" component
        Html(std::string);                  // Constructor with content
        Html(std::string, std::string);     // Constructor with CSS file path

        void addChild(Component *newChild); // Add a new subcomponent to the body
        void setText(std::string text);     // Override the function to set text (there should be no text inside the Html tags)
};

#endif
