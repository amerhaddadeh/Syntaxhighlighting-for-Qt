/****************************************************************************
**This code contain some line of codes from examples that belongs to the following:
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** $QT_BEGIN_LICENSE:BSD$
****************************************************************************/

#include "highlighter.h"

//! [0]
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bADDLW\\b" << "\\bINT\\b" << "\\bSUBLW\\b"
                    << "\\bRETLW\\b" << "\\bNOP\\b" << "\\bOPTION\\b"
                    << "\\bfSLEEP\\b" << "\\bCLRWDT\\b" << "\\bXORLW\\b"
                    << "\\bRETURN\\b" << "\\bRETFIE\\b" << "\\bTRIS\\b"
                    << "\\bMOVWF\\b" << "\\bCLR\\b" << "\\bSUBWF\\b"
                    << "\\bDECF\\b" << "\\bIORWF\\b" << "\\bANDWF\\b"
                    << "\\bXORWF\\b" << "\\bADDWF\\b" << "\\bMOVF\\b"
                    << "\\bCOMF\\b" << "\\bINCF\\b" << "\\bDECFSZ\\b"
                    << "\\bRRF\\b" << "\\bRLF\\b" << "\\bSWAPF\\b"
                    << "\\bBCF\\b" << "\\bINCFSZ\\b" << "\\bBSF\\b"
                    << "\\bBTFSC\\b" << "\\bBTFSS\\b" << "\\bRESET\\b"
                    << "\\bCALL\\b" << "\\bGOTO\\b" << "\\bMOVLW\\b"
                    << "\\bNOP\\b" << "\\bDAA\\b" << "\\bCONTW\\b"
                    << "\\bSLEEP\\b" << "\\bCLRWDT\\b" << "\\bIOW\\b"
                    << "\\bENI\\b" << "\\bDISI\\b" << "\\bRET\\b"
                    << "\\bRETI\\b" << "\\bCONTR\\b" << "\\bIOR\\b"
                    << "\\bCALLW\\b" << "\\bANDLW\\b" << "\\bIORLW\\b"
                    << "\\bSUBWFB\\b" << "\\bADDWFC\\b" << "\\bASRF\\b"
                    << "\\bLSRF\\b" << "\\bLSLF\\b" << "\\bADDFSR\\b"
                    << "\\bBRW\\b" << "\\bMOVLP\\b" << "\\bBRA\\b"
                    << "\\bMOVIW\\b" << "\\bMOVWI\\b" << "\\bMOVIW ++FSRn\\b"
                     << "\\bMOVIW −−FSRn\\b" << "\\bMOVIW FSRn++\\b" << "\\bMOVIW FSRn−−\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

    }

    classFormat.setFontWeight(QFont::Bold); // set type of recognised text (input)
    classFormat.setForeground(Qt::darkMagenta); //set color of the recognised text.(input)
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b"); // regular expression that meant to consider of input text
    rule.format = classFormat;
    highlightingRules.append(rule);  // actual highighting


    // ************* regular exp. for multi-line comment & coloring ****************

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);  // set the foreground (not neccessary)


   // *************** single line coloring & reg. exp. for comment  *****************

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

  // ********* expression coloring

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);


    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

// ********** Iteration for coloring commands in list ******************

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }


    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

// ********* Iteration for colorin comments ****************

    while (startIndex >= 0) {

        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

