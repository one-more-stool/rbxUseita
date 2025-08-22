# Supported Languages

rbxUseita supports 29 languages with native language names in the interface.

## Language List

| Language | Native Name | Code | Status |
|----------|-------------|------|--------|
| English | English | `en` | ✅ Complete |
| Russian | Русский | `ru` | ✅ Complete |
| Finnish | Suomi | `fi` | ✅ Complete |
| Spanish | Español | `es` | ✅ Complete |
| French | Français | `fr` | ✅ Complete |
| German | Deutsch | `de` | ✅ Complete |
| Italian | Italiano | `it` | ✅ Complete |
| Portuguese | Português | `pt` | ✅ Complete |
| Polish | Polski | `pl` | ✅ Complete |
| Dutch | Nederlands | `nl` | ✅ Complete |
| Swedish | Svenska | `sv` | ✅ Complete |
| Norwegian | Norsk | `no` | ✅ Complete |
| Danish | Dansk | `da` | ✅ Complete |
| Czech | Čeština | `cs` | ✅ Complete |
| Hungarian | Magyar | `hu` | ✅ Complete |
| Romanian | Română | `ro` | ✅ Complete |
| Bulgarian | Български | `bg` | ✅ Complete |
| Ukrainian | Українська | `uk` | ✅ Complete |
| Turkish | Türkçe | `tr` | ✅ Complete |
| Vietnamese | Tiếng Việt | `vi` | ✅ Complete |
| Indonesian | Bahasa Indonesia | `id` | ✅ Complete |
| Malay | Bahasa Melayu | `ms` | ✅ Complete |
| Greek | Ελληνικά | `el` | ✅ Complete |
| Croatian | Hrvatski | `hr` | ✅ Complete |
| Slovak | Slovenčina | `sk` | ✅ Complete |
| Slovenian | Slovenščina | `sl` | ✅ Complete |
| Lithuanian | Lietuvių | `lt` | ✅ Complete |
| Latvian | Latviešu | `lv` | ✅ Complete |
| Estonian | Eesti | `et` | ✅ Complete |

## Localized Strings

Each language includes translations for:
- "RBX windows found" - Status message when RBX processes are detected
- "BLOCKING" - Status when a window is blocking new instances
- "All windows OK" - Status when no blocking windows are found
- "Auto-unblock" - Checkbox label for automatic unblocking

## Adding New Languages

To add a new language:

1. Add the language code and native name to the arrays in `ui_manager.cpp`
2. Add the language strings to `localization.cpp` in the `loadLanguages()` function
3. Update this file with the new language information

## Font Support

The application uses system fonts that support the required character sets for each language. For languages with special characters (Cyrillic, CJK, Arabic, etc.), the system will automatically select an appropriate font.
