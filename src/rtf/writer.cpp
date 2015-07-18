/***********************************************************************
 *
 * Copyright (C) 2010, 2011, 2012 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "writer.h"

#include <QHash>
#include <QIODevice>
#include <QLocale>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextDocument>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

//-----------------------------------------------------------------------------

namespace
{

#ifndef Q_OS_WIN32

QHash<QLocale::Language, QByteArray> mapCodePages()
{
	QHash<QLocale::Language, QByteArray> codepages;
	codepages[QLocale::Thai] = "CP874";
	codepages[QLocale::Japanese] = "CP932";
	codepages[QLocale::Korean] = "CP949";
	codepages[QLocale::Albanian] = "CP1250";
	codepages[QLocale::Bosnian] = "CP1250";
	codepages[QLocale::Croatian] = "CP1250";
	codepages[QLocale::Czech] = "CP1250";
	codepages[QLocale::Hungarian] = "CP1250";
	codepages[QLocale::Polish] = "CP1250";
	codepages[QLocale::Romanian] = "CP1250";
	codepages[QLocale::SerboCroatian] = "CP1250";
	codepages[QLocale::Slovak] = "CP1250";
	codepages[QLocale::Slovenian] = "CP1250";
	codepages[QLocale::Turkmen] = "CP1250";
	codepages[QLocale::Bashkir] = "CP1251";
	codepages[QLocale::Bulgarian] = "CP1251";
	codepages[QLocale::Byelorussian] = "CP1251";
	codepages[QLocale::Kazakh] = "CP1251";
	codepages[QLocale::Kirghiz] = "CP1251";
	codepages[QLocale::Macedonian] = "CP1251";
	codepages[QLocale::Mongolian] = "CP1251";
	codepages[QLocale::Russian] = "CP1251";
	codepages[QLocale::Tajik] = "CP1251";
	codepages[QLocale::Tatar] = "CP1251";
	codepages[QLocale::Ukrainian] = "CP1251";
	codepages[QLocale::Afrikaans] = "CP1252";
	codepages[QLocale::Basque] = "CP1252";
	codepages[QLocale::Breton] = "CP1252";
	codepages[QLocale::Corsican] = "CP1252";
	codepages[QLocale::Danish] = "CP1252";
	codepages[QLocale::Dutch] = "CP1252";
	codepages[QLocale::English] = "CP1252";
	codepages[QLocale::Faroese] = "CP1252";
	codepages[QLocale::Finnish] = "CP1252";
	codepages[QLocale::French] = "CP1252";
	codepages[QLocale::Frisian] = "CP1252";
	codepages[QLocale::Galician] = "CP1252";
	codepages[QLocale::Gaelic] = "CP1252";
	codepages[QLocale::German] = "CP1252";
	codepages[QLocale::Greenlandic] = "CP1252";
	codepages[QLocale::Hausa] = "CP1252";
	codepages[QLocale::Icelandic] = "CP1252";
	codepages[QLocale::Igbo] = "CP1252";
	codepages[QLocale::Indonesian] = "CP1252";
	codepages[QLocale::Inuktitut] = "CP1252";
	codepages[QLocale::Irish] = "CP1252";
	codepages[QLocale::Italian] = "CP1252";
	codepages[QLocale::Kinyarwanda] = "CP1252";
	codepages[QLocale::Malay] = "CP1252";
	codepages[QLocale::Norwegian] = "CP1252";
	codepages[QLocale::NorwegianNynorsk] = "CP1252";
	codepages[QLocale::Occitan] = "CP1252";
	codepages[QLocale::Portuguese] = "CP1252";
	codepages[QLocale::RhaetoRomance] = "CP1252";
	codepages[QLocale::Quechua] = "CP1252";
	codepages[QLocale::Spanish] = "CP1252";
	codepages[QLocale::Swahili] = "CP1252";
	codepages[QLocale::Swedish] = "CP1252";
	codepages[QLocale::Tagalog] = "CP1252";
	codepages[QLocale::Welsh] = "CP1252";
	codepages[QLocale::Wolof] = "CP1252";
	codepages[QLocale::Xhosa] = "CP1252";
	codepages[QLocale::Yoruba] = "CP1252";
	codepages[QLocale::Zulu] = "CP1252";
	codepages[QLocale::Greek] = "CP1253";
	codepages[QLocale::Azerbaijani] = "CP1254";
	codepages[QLocale::Turkish] = "CP1254";
	codepages[QLocale::Uzbek] = "CP1254";
	codepages[QLocale::Hebrew] = "CP1255";
	codepages[QLocale::Yiddish] = "CP1255";
	codepages[QLocale::Arabic] = "CP1256";
	codepages[QLocale::Persian] = "CP1256";
	codepages[QLocale::Urdu] = "CP1256";
	codepages[QLocale::Estonian] = "CP1257";
	codepages[QLocale::Latvian] = "CP1257";
	codepages[QLocale::Lithuanian] = "CP1257";
	codepages[QLocale::Vietnamese] = "CP1258";
#if QT_VERSION >= QT_VERSION_CHECK(4, 7, 0)
	codepages[QLocale::CentralMoroccoTamazight] = "CP1252";
	codepages[QLocale::LowGerman] = "CP1252";
	codepages[QLocale::NorthernSami] = "CP1252";
	codepages[QLocale::SwissGerman] = "CP1252";
#endif
	return codepages;
}

QByteArray fetchCodePage()
{
	// Search code page map for current language
	QLocale::Language language = QLocale().language();
	static const QHash<QLocale::Language, QByteArray> codepages = mapCodePages();
	QByteArray codepage = codepages.value(language);

	// Guess at Chinese code page for current country
	if (codepage.isEmpty() && language == QLocale::Chinese) {
		QLocale::Country country = QLocale().country();
		codepage = (country == QLocale::HongKong || country == QLocale::Macau || country == QLocale::Taiwan) ? "CP950" : "CP936";
	}

	// Guess at closest code page from environment variables
	if (codepage.isEmpty()) {
		QByteArray lang = setlocale(LC_CTYPE, 0);
		if (lang.isEmpty() || lang == "C") {
			lang = qgetenv("LC_ALL");
		}
		if (lang.isEmpty() || lang == "C") {
			lang = qgetenv("LC_CTYPE");
		}
		if (lang.isEmpty() || lang == "C") {
			lang = qgetenv("LANG");
		}

		if (language == QLocale::Serbian) {
			codepage = (lang.contains("@latin") || lang.contains("Latn")) ? "CP1250" : "CP1251";
		}

		if (lang.contains("8859-2")) {
			codepage = "CP1250";
		} else if (lang.contains("8859-3")) {
			codepage = "CP1254";
		} else if (lang.contains("8859-4")) {
			codepage = "CP1257";
		} else if (lang.contains("8859-5")) {
			codepage = "CP1251";
		} else if (lang.contains("8859-6")) {
			codepage = "CP1256";
		} else if (lang.contains("8859-7")) {
			codepage = "CP1253";
		} else if (lang.contains("8859-8")) {
			codepage = "CP1255";
		} else if (lang.contains("8859-9")) {
			codepage = "CP1254";
		} else if (lang.contains("8859-11")) {
			codepage = "CP874";
		} else if (lang.contains("8859-13")) {
			codepage = "CP1257";
		}

		if (codepage.isEmpty() && lang.contains("UTF-8")) {
			codepage = "CP65001";
		}
	}

	// Fall back to Western European
	if (codepage.isEmpty()) {
		codepage = "CP1252";
	}

	return codepage;
}

#else

QByteArray fetchCodePage()
{
	TCHAR buffer[7];
	int size = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, buffer, sizeof(buffer) / sizeof(TCHAR));
#if UNICODE
	QString codepage = QLatin1String("CP") + QString::fromUtf16((ushort*)buffer, size - 1);
#else
	QString codepage = QLatin1String("CP") + QString::fromLocal8Bit(buffer);
#endif
    return codepage.toLatin1();
}

#endif

}

//-----------------------------------------------------------------------------

RTF::Writer::Writer(const QByteArray& codepage)
	: m_codepage(codepage),
	  m_codec(0)
{
	// Fetch system codepage
	if (m_codepage.isEmpty()) {
		m_codepage = fetchCodePage();
	}
	if (m_codepage == "CP932") {
		m_codepage = "Shift-JIS";
	}

	// Load codec
	m_codec = QTextCodec::codecForName(m_codepage);
	if (!m_codec) {
		m_codepage = "CP1252";
		m_codec = QTextCodec::codecForName(m_codepage);
	}

	// Create header
	switch (m_codec->mibEnum()) {
	case -168: m_header = "{\\rtf1\\mac\\ansicpg10000\n"; break;
	case 17: m_header = "{\\rtf1\\ansi\\ansicpg932\n"; break;
	case 106: m_header = "{\\rtf1\\ansi\\ansicpg65001\n"; break;
	case 2009: m_header = "{\\rtf1\\pca\\ansicpg850\n"; break;
	default: m_header = "{\\rtf1\\ansi\\ansicpg" + m_codepage.mid(2) + "\n"; break;
	}
}

//-----------------------------------------------------------------------------

bool RTF::Writer::write(QIODevice* device, QTextDocument* text, bool full)
{
	if (m_codec == 0) {
		return false;
	}

	device->write(m_header);

	for (QTextBlock block = text->begin(); block.isValid(); block = block.next()) {
		if (full) {
			QByteArray par("{\\pard\\plain");
			QTextBlockFormat block_format = block.blockFormat();
			bool rtl = block_format.layoutDirection() == Qt::RightToLeft;
			if (rtl) {
				par += "\\rtlpar";
			}
			Qt::Alignment align = block_format.alignment();
			if (rtl && (align & Qt::AlignLeft)) {
				par += "\\ql";
			} else if (align & Qt::AlignRight) {
				par += "\\qr";
			} else if (align & Qt::AlignCenter) {
				par += "\\qc";
			} else if (align & Qt::AlignJustify) {
				par += "\\qj";
			}
			if (block_format.indent() > 0) {
				par += "\\li" + QByteArray::number(block_format.indent() * 15);
			}
			device->write(par);
		} else {
			device->write("{");
		}

		if (block.begin() != block.end()) {
			if (full) {
				device->write(" ");
			}
			for (QTextBlock::iterator iter = block.begin(); !(iter.atEnd()); ++iter) {
				QTextFragment fragment = iter.fragment();
				QTextCharFormat char_format = fragment.charFormat();
				QByteArray style;
				if (char_format.fontWeight() == QFont::Bold) {
					style += "\\b";
				}
				if (char_format.fontItalic()) {
					style += "\\i";
				}
				if (char_format.fontUnderline()) {
					style += "\\ul";
				}
				if (char_format.fontStrikeOut()) {
					style += "\\strike";
				}
				if (char_format.verticalAlignment() == QTextCharFormat::AlignSuperScript) {
					style += "\\super";
				} else if (char_format.verticalAlignment() == QTextCharFormat::AlignSubScript) {
					style += "\\sub";
				}

				if (!style.isEmpty()) {
					device->write("{" + style + " " + fromUnicode(fragment.text()) + "}");
				} else {
					device->write(fromUnicode(fragment.text()));
				}
			}
		}

		if (full || block.next().isValid()) {
			device->write("\\par}\n");
		} else {
			device->write("}");
		}
	}

	device->write("}");
	return true;
}

//-----------------------------------------------------------------------------

QByteArray RTF::Writer::fromUnicode(const QString& string) const
{
	QByteArray text;

	QByteArray encoded;
	QTextCodec::ConverterState state;
	state.flags = QTextCodec::ConvertInvalidToNull;

	QString::const_iterator end = string.constEnd();
	for (QString::const_iterator i = string.constBegin(); i != end; ++i) {
		switch (i->unicode()) {
		case '\t': text += "\\tab "; break;
		case '\\': text += "\\'5C"; break;
		case '{': text += "\\'7B"; break;
		case '}': text += "\\'7D"; break;
		case 0x00a0: text += "\\~"; break;
		case 0x00ad: text += "\\-"; break;
		case 0x00b7: text += "\\|"; break;
		case 0x2002: text += "\\enspace "; break;
		case 0x2003: text += "\\emspace "; break;
		case 0x2004: text += "\\qmspace "; break;
		case 0x200c: text += "\\zwnj "; break;
		case 0x200d: text += "\\zwj "; break;
		case 0x200e: text += "\\ltrmark "; break;
		case 0x200f: text += "\\rtlmark "; break;
		case 0x2011: text += "\\_"; break;
		case 0x2013: text += "\\endash "; break;
		case 0x2014: text += "\\emdash "; break;
		case 0x2018: text += "\\lquote "; break;
		case 0x2019: text += "\\rquote "; break;
		case 0x201c: text += "\\ldblquote "; break;
		case 0x201d: text += "\\rdblquote "; break;
		case 0x2022: text += "\\bullet "; break;
		case 0x2028: text += "\\line "; break;
		default:
			encoded = m_codec->fromUnicode(i, 1, &state);
			if (state.invalidChars == 0) {
				if (encoded.count() == 1 && encoded.at(0) >= 0x20) {
					text += encoded;
				} else {
					for (int j = 0; j < encoded.count(); ++j) {
						text += "\\'" + QByteArray::number(static_cast<unsigned char>(encoded.at(j)), 16).toUpper();
					}
				}
			} else {
				text += "\\u" + QByteArray::number(i->unicode()) + "?";
			}
		}
	}

	return text;
}

//-----------------------------------------------------------------------------
