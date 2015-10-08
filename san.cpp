//
//  main.cpp
//  test
//
//  Created by Weston Dransfield on 10/7/15.
//  Copyright © 2015 Weston Dransfield. All rights reserved.
//

#include <iostream>
#include <string>
#include <set>

using namespace std;

/*
* class Element
* Represents an HTML element and inner HTML.
* Public Member Functions
*  Element(string HTML) - Initialize an Element with the proved HTML
*  getHtml - Recursively display an element and its child elements, encoding
unsafe HTML tags while recursing down.
* Private Member Functions
*  initWhiteList - Initializes the whitelist with safe element names
*  setTagData - Sets the tag name and initializes other Element data
*  validateClosingTag - Checks to see if the current Element has a closing tag.
If it does not, a closing tag is added to the HTML.
*  setInnerHtml - sets the inner HTML of the current element.
*  setPrePostElemText - Isolates characters before the current element and stores
them. Also Isolates characters after the current element
and stores them.
*/
class Element
{
public:
  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  Element(string html)
  {
    //create the whitelist
    safeTags = set<string>();

    //assume the tag closes
    hasClosing = true;

    //assume no attributes
    hasAttributes = false;

    //set the full html string
    this->html = html;

    //set the tag name
    setTagData();

    //make sure there is a closing tag, add it if it's missing and valid
    validateClosingTag();

    //set the string before and after the current tag
    setPrePostElemText();

    //set innerHtml
    setInnerHtml();

    //initialize whitelist
    initWhitelist();
  }

  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  string getHtml()
  {
    if (!isElement)
    return innerHtml;
    if (safeTags.find(name) != safeTags.end())
    return preTag + "<" + name + (hasAttributes ? " " : ">") + Element(innerHtml).getHtml() + "</" + name + ">" + postTag;
    else
    return preTag + "&lt;" + name + "&gt;" + Element(innerHtml).getHtml() + (hasClosing ? "&lt;/" + name + "&gt;" : "") + postTag;
  }

  /*
  * Getters
  */
  bool getIsElement() {return isElement;}
  string getName() {return name;}
  string getInnerHtml() {return innerHtml;}

private:
  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  void initWhitelist()
  {
    //whitelist of safe tags
    safeTags.insert("a");
    safeTags.insert("abbr");
    safeTags.insert("acronym");
    safeTags.insert("b");
    safeTags.insert("blockquote");
    safeTags.insert("cite");
    safeTags.insert("code");
    safeTags.insert("del");
    safeTags.insert("em");
    safeTags.insert("i");
    safeTags.insert("q");
    safeTags.insert("q");
    safeTags.insert("strike");
    safeTags.insert("strong");
  }

  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  void setTagData()
  {
    //if the html does not contain any tags
    if (html.find('<') == string::npos)
    {
      isElement = false;
      return;
    }

    //isolate the tag name
    name = html.substr(html.find('<') + 1);
    name = name.substr(0,name.find('>'));

    if (name.find(' ') != string::npos)
    {
      name = name.substr(0, name.find(' '));
      hasAttributes = true;
    }

    //set string position
    stringPos = html.find('<');

    //this is an element
    isElement = true;
  }

  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  void validateClosingTag()
  {
    //do nothing if the 'element' is not an element
    if (!isElement)
    {
      return;
    }

    //check for a closing tag
    string closing = html.substr(html.rfind('<') + 2, this->name.length());

    //if the closing tag does not exist
    if (closing != name)
    {
      //current element has no closing tag
      hasClosing = false;

      //add a closing tag
      html += "</" + name + ">";
    }

    //set the position of the closing tag
    stringEndPos = html.rfind('<');
  }

  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  void setInnerHtml()
  {
    //if the current element is not really an element
    if (!isElement)
    {
      //inner html is the full html
      innerHtml = html;
      return;
    }

    //isolate the content within the current tag and set it as innerHtml
    innerHtml = html.substr(stringPos + name.length() + 2);
    innerHtml = innerHtml.substr(0, innerHtml.length() - (name.length() + 3 + postTag.length()));
  }

  /*
  * function getHtml
  *
  * Description:
  * Recursively returns an element and its child elements as a string,
  * encoding unsafe HTML tags while recursing down.
  */
  void setPrePostElemText()
  {
    //if the current element really is an element
    if (isElement)
    {
      //isolate the text before the current element and store it
      preTag = html.substr(0, stringPos);

      //isolate the text after the current element and store it
      postTag = html.substr(stringEndPos + 3 + name.length());
    }
  }

  string name;      //name of the tag
  string innerHtml; //the html nested within the tag
  string preTag;    //text before the current tag
  string postTag;   //text after the current tag
  string html;      //the full html (including post and pre tag characters)
  int stringPos;    //position of the tag within html
  int stringEndPos; //position of the ending tag within html
  bool isElement;   //is this an element or a plain old string?
  bool hasClosing;  //does the element have a closing tag?
  bool hasAttributes; //does the element have attributes?
  set<string> safeTags; //set of safe tag names
};

class Sanitizer
{
public:
  Sanitizer()
  {

  }

  string sanitize(Element currentElem)
  {
    return currentElem.getHtml();
  }

  void run()
  {
    //get user input
    while (html != "quit")
    {
      promptUser();
      Element currentElem = Element(html);
      cout << "\t" << sanitize(currentElem) << endl;
    }
  }

  void promptUser()
  {
    cout << "> ";
    getline(cin, html);
  }
private:
  string html;
  string cleanHtml;
};

int main()
{
  Sanitizer mainSanitizer = Sanitizer();
  mainSanitizer.run();
}
