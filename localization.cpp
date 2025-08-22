#include "localization.h"

Localization& Localization::getInstance() {
    static Localization instance;
    return instance;
}

Localization::Localization() {
    loadLanguages();
    setLanguage("en"); // Default to English
}

void Localization::loadLanguages() {
    // English
    LocalizedStrings en;
    en.robloxWindow = "RBX windows found";
    en.blocking = "BLOCKING";
    en.ok = "All windows OK";
    en.autoUnblock = "Auto-unblock";
    languages["en"] = en;
    
    // Russian
    LocalizedStrings ru;
    ru.robloxWindow = "Найдено окон RBX";
    ru.blocking = "БЛОКИРУЕТ";
    ru.ok = "Все окна ОК";
    ru.autoUnblock = "Авторазблокировка";
    languages["ru"] = ru;
    
    // Finnish
    LocalizedStrings fi;
    fi.robloxWindow = "RBX-ikkunoita löytyi";
    fi.blocking = "ESITTÄÄ";
    fi.ok = "Kaikki ikkunat OK";
    fi.autoUnblock = "Automaattinen esto poisto";
    languages["fi"] = fi;
    
    // Spanish
    LocalizedStrings es;
    es.robloxWindow = "Ventanas RBX encontradas";
    es.blocking = "BLOQUEANDO";
    es.ok = "Todas las ventanas OK";
    es.autoUnblock = "Auto-desbloquear";
    languages["es"] = es;
    
    // French
    LocalizedStrings fr;
    fr.robloxWindow = "Fenêtres RBX trouvées";
    fr.blocking = "BLOQUE";
    fr.ok = "Toutes les fenêtres OK";
    fr.autoUnblock = "Auto-débloquer";
    languages["fr"] = fr;
    
    // German
    LocalizedStrings de;
    de.robloxWindow = "RBX-Fenster gefunden";
    de.blocking = "BLOCKIERT";
    de.ok = "Alle Fenster OK";
    de.autoUnblock = "Auto-Entsperren";
    languages["de"] = de;
    
    // Italian
    LocalizedStrings it;
    it.robloxWindow = "Finestre RBX trovate";
    it.blocking = "BLOCCANTE";
    it.ok = "Tutte le finestre OK";
    it.autoUnblock = "Auto-sblocca";
    languages["it"] = it;
    
    // Portuguese
    LocalizedStrings pt;
    pt.robloxWindow = "Janelas RBX encontradas";
    pt.blocking = "BLOQUEANDO";
    pt.ok = "Todas as janelas OK";
    pt.autoUnblock = "Auto-desbloquear";
    languages["pt"] = pt;
    
    // Polish
    LocalizedStrings pl;
    pl.robloxWindow = "Znalezione okna RBX";
    pl.blocking = "BLOKUJE";
    pl.ok = "Wszystkie okna OK";
    pl.autoUnblock = "Auto-odblokuj";
    languages["pl"] = pl;
    
    // Dutch
    LocalizedStrings nl;
    nl.robloxWindow = "RBX-vensters gevonden";
    nl.blocking = "BLOKKEERT";
    nl.ok = "Alle vensters OK";
    nl.autoUnblock = "Auto-ontgrendelen";
    languages["nl"] = nl;
    
    // Swedish
    LocalizedStrings sv;
    sv.robloxWindow = "RBX-fönster hittade";
    sv.blocking = "BLOCKERAR";
    sv.ok = "Alla fönster OK";
    sv.autoUnblock = "Auto-avblockera";
    languages["sv"] = sv;
    
    // Norwegian
    LocalizedStrings no;
    no.robloxWindow = "RBX-vinduer funnet";
    no.blocking = "BLOKKERER";
    no.ok = "Alle vinduer OK";
    no.autoUnblock = "Auto-opplås";
    languages["no"] = no;
    
    // Danish
    LocalizedStrings da;
    da.robloxWindow = "RBX-vinduer fundet";
    da.blocking = "BLOKERER";
    da.ok = "Alle vinduer OK";
    da.autoUnblock = "Auto-oplås";
    languages["da"] = da;
    
    // Czech
    LocalizedStrings cs;
    cs.robloxWindow = "Nalezena RBX okna";
    cs.blocking = "BLOKUJE";
    cs.ok = "Všechna okna OK";
    cs.autoUnblock = "Auto-odblokovat";
    languages["cs"] = cs;
    
    // Hungarian
    LocalizedStrings hu;
    hu.robloxWindow = "RBX ablakok találva";
    hu.blocking = "BLOKKOL";
    hu.ok = "Minden ablak OK";
    hu.autoUnblock = "Auto-feloldás";
    languages["hu"] = hu;
    
    // Romanian
    LocalizedStrings ro;
    ro.robloxWindow = "Ferestre RBX găsite";
    ro.blocking = "BLOCHENDAZĂ";
    ro.ok = "Toate ferestrele OK";
    ro.autoUnblock = "Auto-deblocare";
    languages["ro"] = ro;
    
    // Bulgarian
    LocalizedStrings bg;
    bg.robloxWindow = "Намерени RBX прозорци";
    bg.blocking = "БЛОКИРА";
    bg.ok = "Всички прозорци OK";
    bg.autoUnblock = "Авто-отключване";
    languages["bg"] = bg;
    
    // Ukrainian
    LocalizedStrings uk;
    uk.robloxWindow = "Знайдено вікон RBX";
    uk.blocking = "БЛОКУЄ";
    uk.ok = "Всі вікна OK";
    uk.autoUnblock = "Авторозблокування";
    languages["uk"] = uk;
    
    // Turkish
    LocalizedStrings tr;
    tr.robloxWindow = "RBX pencereleri bulundu";
    tr.blocking = "ENGELLİYOR";
    tr.ok = "Tüm pencereler OK";
    tr.autoUnblock = "Otomatik engel kaldır";
    languages["tr"] = tr;
    
    // Vietnamese
    LocalizedStrings vi;
    vi.robloxWindow = "Tìm thấy cửa sổ RBX";
    vi.blocking = "ĐANG CHẶN";
    vi.ok = "Tất cả cửa sổ OK";
    vi.autoUnblock = "Tự động bỏ chặn";
    languages["vi"] = vi;
    
    // Indonesian
    LocalizedStrings id;
    id.robloxWindow = "Jendela RBX ditemukan";
    id.blocking = "MEMBLOKIR";
    id.ok = "Semua jendela OK";
    id.autoUnblock = "Auto-buka blokir";
    languages["id"] = id;
    
    // Malay
    LocalizedStrings ms;
    ms.robloxWindow = "Tetingkap RBX dijumpai";
    ms.blocking = "MENYEKAT";
    ms.ok = "Semua tetingkap OK";
    ms.autoUnblock = "Auto-buka sekatan";
    languages["ms"] = ms;
    
    // Greek
    LocalizedStrings el;
    el.robloxWindow = "Βρέθηκαν παράθυρα RBX";
    el.blocking = "ΑΠΑΓΟΡΕΥΕΙ";
    el.ok = "Όλα τα παράθυρα OK";
    el.autoUnblock = "Αυτόματη άρση αποκλεισμού";
    languages["el"] = el;
    
    // Croatian
    LocalizedStrings hr;
    hr.robloxWindow = "Pronađeni RBX prozori";
    hr.blocking = "BLOKIRA";
    hr.ok = "Svi prozori OK";
    hr.autoUnblock = "Auto-odblokiraj";
    languages["hr"] = hr;
    
    // Slovak
    LocalizedStrings sk;
    sk.robloxWindow = "Nájdené RBX okná";
    sk.blocking = "BLOKUJE";
    sk.ok = "Všetky okná OK";
    sk.autoUnblock = "Auto-odblokovať";
    languages["sk"] = sk;
    
    // Slovenian
    LocalizedStrings sl;
    sl.robloxWindow = "Najdena RBX okna";
    sl.blocking = "BLOKIRA";
    sl.ok = "Vsa okna OK";
    sl.autoUnblock = "Auto-odblokiraj";
    languages["sl"] = sl;
    
    // Lithuanian
    LocalizedStrings lt;
    lt.robloxWindow = "Rasti RBX langai";
    lt.blocking = "BLOKUOJA";
    lt.ok = "Visi langai OK";
    lt.autoUnblock = "Auto-atskirti";
    languages["lt"] = lt;
    
    // Latvian
    LocalizedStrings lv;
    lv.robloxWindow = "Atrasti RBX logi";
    lv.blocking = "BLOKĒ";
    lv.ok = "Visi logi OK";
    lv.autoUnblock = "Auto-atslēgt";
    languages["lv"] = lv;
    
    // Estonian
    LocalizedStrings et;
    et.robloxWindow = "Leitud RBX aknad";
    et.blocking = "BLOKEERIB";
    et.ok = "Kõik aknad OK";
    et.autoUnblock = "Auto-lukusta lahti";
    languages["et"] = et;
}

void Localization::setLanguage(const std::string& language) {
    auto it = languages.find(language);
    if (it != languages.end()) {
        currentLanguage = language;
        currentStrings = it->second;
    }
}

const LocalizedStrings& Localization::getStrings() const {
    return currentStrings;
}

std::string Localization::getLanguage() const {
    return currentLanguage;
}
