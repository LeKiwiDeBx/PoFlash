#include <stdio.h>

int main() {
    const char* languages[] = {
        "ab", // Abkhazian
        "aa", // Afar
        "af", // Afrikaans
        "ak", // Akan
        "sq", // Albanian
        "am", // Amharic
        "ar", // Arabic
        "an", // Aragonese
        "hy", // Armenian
        "as", // Assamese
        "av", // Avaric
        "ae", // Avestan
        "ay", // Aymara
        "az", // Azerbaijani
        "bm", // Bambara
        "ba", // Bashkir
        "eu", // Basque
        "be", // Belarusian
        "bn", // Bengali
        "bh", // Bihari
        "bi", // Bislama
        "bs", // Bosnian
        "br", // Breton
        "bg", // Bulgarian
        "my", // Burmese
        "ca", // Catalan
        "ch", // Chamorro
        "ce", // Chechen
        "zh", // Chinese
        "cu", // Church Slavic
        "cv", // Chuvash
        "kw", // Cornish
        "co", // Corsican
        "cr", // Cree
        "hr", // Croatian
        "cs", // Czech
        "da", // Danish
        "dv", // Divehi
        "nl", // Dutch
        "dz", // Dzongkha
        "en", // English
        "eo", // Esperanto
        "et", // Estonian
        "ee", // Ewe
        "fo", // Faroese
        "fj", // Fijian
        "fi", // Finnish
        "fr", // French
        "ff", // Fulah
        "gl", // Galician
        "ka", // Georgian
        "de", // German
        "el", // Greek
        "gn", // Guarani
        "gu", // Gujarati
        "ht", // Haitian
        "ha", // Hausa
        "he", // Hebrew
        "hz", // Herero
        "hi", // Hindi
        "ho", // Hiri Motu
        "hu", // Hungarian
        "ia", // Interlingua
        "id", // Indonesian
        "ie", // Interlingue
        "ga", // Irish
        "ig", // Igbo
        "ik", // Inupiaq
        "io", // Ido
        "is", // Icelandic
        "it", // Italian
        "iu", // Inuktitut
        "ja", // Japanese
        "jv", // Javanese
        "kl", // Kalaallisut
        "kn", // Kannada
        "kr", // Kanuri
        "ks", // Kashmiri
        "kk", // Kazakh
        "km", // Khmer
        "ki", // Kikuyu
        "rw", // Kinyarwanda
        "ky", // Kyrgyz
        "kv", // Komi
        "kg", // Kongo
        "ko", // Korean
        "kj", // Kuanyama
        "ku", // Kurdish
        "lo", // Lao
        "la", // Latin
        "lv", // Latvian
        "li", // Limburgish
        "ln", // Lingala
        "lt", // Lithuanian
        "lb", // Luxembourgish
        "lu", // Luba-Katanga
        "mk", // Macedonian
        "mg", // Malagasy
        "ms", // Malay
        "ml", // Malayalam
        "mt", // Maltese
        "gv", // Manx
        "mi", // Maori
        "mr", // Marathi
        "mh", // Marshallese
        "mn", // Mongolian
        "na", // Nauru
        "nv", // Navajo
        "nd", // North Ndebele
        "ne", // Nepali
        "ng", // Ndonga
        "nb", // Norwegian Bokmål
        "nn", // Norwegian Nynorsk
        "no", // Norwegian
        "ii", // Sichuan Yi
        "nr", // South Ndebele
        "oc", // Occitan
        "oj", // Ojibwa
        "or", // Oriya
        "om", // Oromo
        "os", // Ossetian
        "pa", // Panjabi
        "pi", // Pali
        "fa", // Persian
        "pl", // Polish
        "ps", // Pashto
        "pt", // Portuguese
        "qu", // Quechua
        "rm", // Romansh
        "rn", // Rundi
        "ro", // Romanian
        "ru", // Russian
        "sa", // Sanskrit
        "sc", // Sardinian
        "sd", // Sindhi
        "se", // Northern Sami
        "sm", // Samoan
        "sg", // Sango
        "sr", // Serbian
        "gd", // Scottish Gaelic
        "sn", // Shona
        "si", // Sinhala
        "sk", // Slovak
        "sl", // Slovenian
        "so", // Somali
        "st", // Southern Sotho
        "es", // Spanish
        "su", // Sundanese
        "sw", // Swahili
        "ss", // Swati
        "sv", // Swedish
        "ta", // Tamil
        "te", // Telugu
        "tg", // Tajik
        "th", // Thai
        "ti", // Tigrinya
        "bo", // Tibetan
        "tk", // Turkmen
        "tl", // Tagalog
        "tn", // Tswana
        "to", // Tonga
        "tr", // Turkish
        "ts", // Tsonga
        "tt", // Tatar
        "tw", // Twi
        "ty", // Tahitian
        "ug", // Uighur
        "uk", // Ukrainian
        "ur", // Urdu
        "uz", // Uzbek
        "ve", // Venda
        "vi", // Vietnamese
        "vo", // Volapük
        "wa", // Walloon
        "cy", // Welsh
        "wo", // Wolof
        "xh", // Xhosa
        "yi", // Yiddish
        "yo", // Yoruba
        "za", // Zhuang
        "zu"  // Zulu
    };

    // Printing the array elements
    for (int i = 0; i < sizeof(languages) / sizeof(languages[0]); i++) {
        printf("%s\n", languages[i]);
    }

    return 0;
}
