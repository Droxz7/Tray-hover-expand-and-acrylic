// ==WindhawkMod==
// @id              tray-hover-expand
// @name            Tray hover expand and acrylic
// @description     Open the hidden system-tray icons popup on hover and give it a customizable translucent acrylic background
// @version         1.9
// @author          Jamz
// @github          https://github.com/Droxz7
// @include         explorer.exe
// @architecture    x86-64
// @compilerOptions -lole32 -loleaut32 -lshell32 -lruntimeobject
// @license         MIT
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Tray hover expand

![Demo](https://i.imgur.com/qsNUNpj.png)

Makes the Windows 11 **hidden system-tray icons popup** faster to use and gives
it a translucent acrylic background that matches other Windows 11 flyouts.

Move the pointer over the **Show hidden icons** chevron (`^`) to open the popup.
You can also have it close automatically after the pointer leaves the chevron
and popup. Clicking the chevron continues to work normally.

This mod affects only the hidden-icons popup. It does **not** make the taskbar
translucent, hide the notification area, or modify Quick Settings, the calendar,
notifications, or the Start menu.

## Features

- Opens the standard Windows hidden-icons popup on hover.
- Optionally closes it after the pointer leaves, with an adjustable delay.
- Applies a real Windows `HostBackdrop` acrylic brush to the popup's existing
  XAML background, without placing a second square window behind it.
- Lets you adjust the darkness and luminosity of the acrylic effect.
- Supports localized Windows installations through editable chevron-name
  keywords and an automatic fallback.
- Can avoid opening over fullscreen apps and can hide the chevron's **Hide**
  tooltip while the popup is open.

## Requirements and compatibility

- Designed for Windows 11 and the standard Windows 11 system tray.
- At least one notification icon must be hidden so that the `^` chevron exists.
- Windows **Transparency effects** should be enabled. If transparency is disabled
  or unavailable, Windows can show the brush's solid fallback colour instead.
- The mod targets `explorer.exe`. Restarting Windows Explorer may be necessary
  after the first installation or after a Windows update.

The popup is an internal Windows XAML component, so a future Windows update may
rename its elements, symbols, or window classes. The advanced settings provide
fallback values for some of these changes.

## Settings

### Hover behaviour

| Setting | Default | What it changes |
| --- | ---: | --- |
| **Collapse when the cursor leaves** (`autoClose`) | On | Closes the popup after the pointer leaves both the chevron and the popup. Turn it off if you want the popup to remain open until you click elsewhere. |
| **Polling interval** (`pollInterval`) | 50 ms | How often the pointer position is checked. Lower values react faster but run the check more often. Values below 10 ms are limited to 10 ms. A range of 30–100 ms normally works well. |
| **Collapse delay** (`grace`) | 200 ms | How long the pointer may remain outside before the popup closes. `0` closes immediately; 150–400 ms helps prevent accidental closing and flicker. |
| **Hit area padding** (`pad`) | 4 px | Expands the hover target around the chevron. Increase it if the chevron is difficult to trigger; very large values can open the popup unintentionally. |
| **Do not activate over fullscreen apps** (`suppressInFullscreen`) | On | Prevents the popup from appearing over fullscreen videos, presentations, or games. Turn it off if you always want hover activation. |
| **Hide the chevron tooltip** (`hideTooltip`) | Off | Hides the Windows **Hide** tooltip while the popup is open, which can otherwise cover icons near the bottom. |

### Acrylic appearance

| Setting | Default | What it changes |
| --- | ---: | --- |
| **Background tint opacity** (`backgroundTintOpacity`) | 28 | Strength of the black tint, from 0 to 100. Lower values reveal more of the desktop; higher values make the popup darker and more solid. |
| **Background luminosity** (`backgroundLuminosityOpacity`) | 55 | How strongly the backdrop's light and colour contribute to the acrylic material, from 0 to 100. This changes the frosted appearance; it is not a direct blur-radius control. |

Suggested starting points:

- **Balanced:** tint 28, luminosity 55 (defaults).
- **Clearer/lighter:** tint 12, luminosity 45.
- **Darker:** tint 45, luminosity 60.

The exact result depends on the wallpaper, Windows theme, and system transparency
settings. Changes are reapplied to popup backgrounds that the mod has already
found.

### Detection and compatibility (advanced)

| Setting | Default | What it changes |
| --- | --- | --- |
| **Chevron name keywords** (`keywords`) | English and Polish terms | Case-insensitive pieces of the chevron's accessible name. Add the wording used by your Windows display language if hovering does not find the button. |
| **Tray icon AutomationId** (`trayIconAutomationId`) | `SystemTrayIcon` | Fallback identifier used when no keyword matches. The leftmost matching tray button is treated as the chevron. Normally this should not be changed. |
| **Flyout window class** (`flyoutClass`) | `TopLevelWindowForOverflowXamlIsland` | Used to detect whether the pointer is over the open popup. Change it only if auto-close stops working on a particular Windows build. |
| **Tooltip window class** (`tooltipClass`) | `Xaml_WindowedPopupClass` | Used only by **Hide the chevron tooltip**. Change it only if tooltip hiding stops working on a particular Windows build. |

## How chevron detection works

Windows 11 does not expose a unique, language-independent identifier for the
Show hidden icons chevron. It shares the `SystemTrayIcon` AutomationId with
other notification-area controls and does not expose an ExpandCollapse pattern.
The mod therefore:

1. Searches for a tray button whose accessible name contains one of the
   configured keywords.
2. If no name matches, uses the leftmost tray button with the configured
   AutomationId as a fallback.
3. Invokes the standard button through UI Automation, so Windows itself opens
   the normal popup.

## Troubleshooting

- **Hover does nothing:** add your language's name for Show hidden icons to
  `keywords`. Also check whether a fullscreen app is active and whether
  `suppressInFullscreen` is enabled.
- **The popup opens but does not close automatically:** verify `autoClose`, then
  check `flyoutClass` if your Windows build uses a different class name.
- **The background is solid:** enable Windows Transparency effects, restart
  Windows Explorer, then reopen the popup.
- **The tooltip still appears:** enable `hideTooltip`; if necessary, verify the
  advanced `tooltipClass` value for your Windows build.
- **After a Windows update:** enable Windhawk logging, restart Explorer, open the
  popup once, and check the mod's log output for symbol or XAML-element errors.

## Uninstalling

Disable or remove the mod in Windhawk and restart Windows Explorer. The original
popup background is restored when the mod unloads.

---
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- autoClose: true
  $name: Collapse when the cursor leaves
  $name:pl-PL: Zwijaj po odjechaniu kursorem
  $description: After the cursor leaves the opened icons (and the chevron), the flyout closes itself.
  $description:pl-PL: Gdy kursor opuści otwarte ikony (i strzałkę), schowek sam się zamyka.
- pollInterval: 50
  $name: Polling interval (ms)
  $name:pl-PL: Częstotliwość sprawdzania (ms)
  $description: How often to check the cursor position. Lower = smoother, more CPU.
  $description:pl-PL: Co ile sprawdzać pozycję kursora. Mniej = płynniej, większe użycie CPU.
- grace: 200
  $name: Collapse delay (ms)
  $name:pl-PL: Opóźnienie zwijania (ms)
  $description: How long the cursor must stay outside the area before the flyout closes (prevents flicker).
  $description:pl-PL: Jak długo kursor musi pozostawać poza obszarem, zanim schowek się zamknie (zapobiega miganiu).
- pad: 4
  $name: Hit area padding (pixels)
  $name:pl-PL: Margines obszaru najechania (piksele)
  $description: Enlarges the hover area around the chevron button.
  $description:pl-PL: Powiększa obszar najechania wokół przycisku strzałki.
- keywords: ["ukryte ikony", "hidden icons", "rozwiń", "overflow"]
  $name: Chevron name keywords
  $name:pl-PL: Słowa kluczowe nazwy strzałki
  $description: Case-insensitive substrings used to match the chevron button name. Add your locale's name for "Show Hidden Icons" here.
  $description:pl-PL: Fragmenty nazwy przycisku strzałki (wielkość liter bez znaczenia). Dodaj tu nazwę „Pokaż ukryte ikony" w swoim języku.
- suppressInFullscreen: true
  $name: Do not activate over fullscreen apps
  $name:pl-PL: Nie aktywuj na aplikacjach pełnoekranowych
  $description: When a fullscreen app is in the foreground (e.g. a fullscreen video or a game), hovering the chevron's location will not open the flyout, so it can't pop up over the content.
  $description:pl-PL: Gdy na pierwszym planie jest aplikacja pełnoekranowa (np. film na pełnym ekranie lub gra), najechanie w miejsce strzałki nie otworzy schowka, więc nie wyskoczy on na wierzchu treści.
- hideTooltip: false
  $name: Hide the chevron tooltip
  $name:pl-PL: Ukryj podpowiedź strzałki
  $description: While the flyout is open and the cursor is on the chevron, hide the "Hide" tooltip that Windows shows over the chevron (it can cover the bottom row of icons).
  $description:pl-PL: Gdy schowek jest otwarty, a kursor jest na strzałce, ukrywa podpowiedź „Ukryj", którą Windows pokazuje nad strzałką (potrafi zasłaniać dolny rząd ikon).
- flyoutClass: TopLevelWindowForOverflowXamlIsland
  $name: Flyout window class
  $name:pl-PL: Klasa okna schowka
  $description: Window class name of the opened flyout (used to tell whether the cursor is over the icons). Change it if auto-collapse does not work.
  $description:pl-PL: Nazwa klasy okna otwartego schowka (służy do sprawdzania, czy kursor jest nad ikonami). Zmień ją, jeśli auto-zwijanie nie działa.
- tooltipClass: Xaml_WindowedPopupClass
  $name: Tooltip window class
  $name:pl-PL: Klasa okna podpowiedzi
  $description: Window class of the chevron tooltip, hidden only when "Hide the chevron tooltip" is enabled. Change it if hiding does not work on your build.
  $description:pl-PL: Klasa okna podpowiedzi strzałki, ukrywanej tylko gdy włączono „Ukryj podpowiedź strzałki". Zmień ją, jeśli ukrywanie nie działa na Twojej kompilacji.
- backgroundTintOpacity: 28
  $name: Background tint opacity
  $description: Strength of the black acrylic tint from 0 to 100. Lower values reveal more of the desktop; higher values make the popup darker and more solid.
- backgroundLuminosityOpacity: 55
  $name: Background luminosity
  $description: How strongly the backdrop's light and colour contribute to the acrylic material, from 0 to 100. This is not a direct blur-radius control.
- trayIconAutomationId: SystemTrayIcon
  $name: Tray icon AutomationId (fallback)
  $name:pl-PL: AutomationId ikony zasobnika (mechanizm zapasowy)
  $description: Used only when the chevron is not matched by name. The leftmost tray button with this AutomationId is then assumed to be the chevron.
  $description:pl-PL: Używane tylko, gdy strzałki nie uda się dopasować po nazwie. Za strzałkę uznawany jest wtedy pierwszy od lewej przycisk zasobnika o tym AutomationId.
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <shellapi.h>
#include <uiautomation.h>
#include <windhawk_utils.h>

#undef GetCurrentTime

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Media.h>

#include <atomic>
#include <iterator>
#include <list>
#include <string>
#include <vector>
#include <algorithm>

#ifndef __IUIAutomation_FWD_DEFINED__
#error "UI Automation headers are missing"
#endif

struct Settings {
    bool autoClose = true;
    int pollInterval = 50;
    int grace = 200;
    int pad = 4;
    bool hideTooltip = false;
    bool suppressInFullscreen = true;
    std::wstring flyoutClass = L"TopLevelWindowForOverflowXamlIsland";
    std::wstring tooltipClass = L"Xaml_WindowedPopupClass";
    std::wstring trayIconAutomationId = L"SystemTrayIcon";
    int flyoutOpacity = 220;
    int acrylicTint = 65;
    std::vector<std::wstring> keywords = {
        L"ukryte ikony", L"hidden icons", L"rozwiń", L"overflow"
    };
};

// g_settings is guarded by g_settingsLock; the worker thread keeps a private
// snapshot and refreshes it when g_settingsGeneration changes, so it never
// reads the strings while the settings thread reassigns them.
static Settings g_settings;
static SRWLOCK g_settingsLock = SRWLOCK_INIT;
static std::atomic<int> g_settingsGeneration{0};

static std::atomic<bool> g_running{false};
static HANDLE g_thread = nullptr;
static HANDLE g_stopEvent = nullptr;

// ---- Flyout translucency ----

enum ACCENT_STATE {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_BLURBEHIND = 3,
    ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
};

struct ACCENT_POLICY {
    ACCENT_STATE AccentState;
    DWORD AccentFlags;
    DWORD GradientColor;
    DWORD AnimationId;
};

enum WINDOWCOMPOSITIONATTRIB {
    WCA_ACCENT_POLICY = 19,
};

struct WINDOWCOMPOSITIONATTRIBDATA {
    WINDOWCOMPOSITIONATTRIB Attrib;
    PVOID pvData;
    SIZE_T cbData;
};

using SetWindowCompositionAttribute_t =
    BOOL(WINAPI*)(HWND, WINDOWCOMPOSITIONATTRIBDATA*);

static HWND g_styledFlyout = nullptr;
static LONG_PTR g_originalFlyoutExStyle = 0;
static bool g_savedFlyoutStyle = false;
static bool g_originalLayeredAttributesValid = false;
static COLORREF g_originalLayeredColorKey = 0;
static BYTE g_originalLayeredAlpha = 255;
static DWORD g_originalLayeredFlags = 0;

static SetWindowCompositionAttribute_t GetSetWindowCompositionAttribute() {
    static SetWindowCompositionAttribute_t function =
        reinterpret_cast<SetWindowCompositionAttribute_t>(
            GetProcAddress(GetModuleHandleW(L"user32.dll"),
                           "SetWindowCompositionAttribute"));
    return function;
}

static void RestoreFlyoutAppearance() {
    if (!g_styledFlyout || !IsWindow(g_styledFlyout)) {
        g_styledFlyout = nullptr;
        g_savedFlyoutStyle = false;
        return;
    }

    if (auto setComposition = GetSetWindowCompositionAttribute()) {
        ACCENT_POLICY policy{};
        policy.AccentState = ACCENT_DISABLED;

        WINDOWCOMPOSITIONATTRIBDATA data{};
        data.Attrib = WCA_ACCENT_POLICY;
        data.pvData = &policy;
        data.cbData = sizeof(policy);
        setComposition(g_styledFlyout, &data);
    }

    if (g_originalLayeredAttributesValid) {
        SetLayeredWindowAttributes(g_styledFlyout,
                                   g_originalLayeredColorKey,
                                   g_originalLayeredAlpha,
                                   g_originalLayeredFlags);
    } else {
        SetLayeredWindowAttributes(g_styledFlyout, 0, 255, LWA_ALPHA);
    }

    if (g_savedFlyoutStyle) {
        SetWindowLongPtrW(g_styledFlyout, GWL_EXSTYLE,
                          g_originalFlyoutExStyle);
        SetWindowPos(g_styledFlyout, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                         SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }

    g_styledFlyout = nullptr;
    g_savedFlyoutStyle = false;
    g_originalLayeredAttributesValid = false;
}

static void ApplyFlyoutTranslucency(HWND flyout, const Settings& s) {
    if (!flyout || !IsWindow(flyout)) return;

    if (g_styledFlyout != flyout) {
        RestoreFlyoutAppearance();

        g_styledFlyout = flyout;
        g_originalFlyoutExStyle =
            GetWindowLongPtrW(flyout, GWL_EXSTYLE);
        g_savedFlyoutStyle = true;

        if (g_originalFlyoutExStyle & WS_EX_LAYERED) {
            g_originalLayeredAttributesValid =
                GetLayeredWindowAttributes(flyout,
                                           &g_originalLayeredColorKey,
                                           &g_originalLayeredAlpha,
                                           &g_originalLayeredFlags) != FALSE;
        }

        SetWindowLongPtrW(flyout, GWL_EXSTYLE,
                          g_originalFlyoutExStyle | WS_EX_LAYERED);
        SetWindowPos(flyout, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                         SWP_NOACTIVATE | SWP_FRAMECHANGED);
    }

    int opacity = std::clamp(s.flyoutOpacity, 0, 255);
    int tint = std::clamp(s.acrylicTint, 0, 255);

    // GradientColor uses the AABBGGRR byte order. The tint is black, so only
    // its alpha byte needs to be set.
    if (auto setComposition = GetSetWindowCompositionAttribute()) {
        ACCENT_POLICY policy{};
        policy.AccentState = ACCENT_ENABLE_ACRYLICBLURBEHIND;
        policy.AccentFlags = 2;
        policy.GradientColor = static_cast<DWORD>(tint) << 24;

        WINDOWCOMPOSITIONATTRIBDATA data{};
        data.Attrib = WCA_ACCENT_POLICY;
        data.pvData = &policy;
        data.cbData = sizeof(policy);
        setComposition(flyout, &data);
    }

    SetLayeredWindowAttributes(flyout, 0,
                               static_cast<BYTE>(opacity), LWA_ALPHA);
    RedrawWindow(flyout, nullptr, nullptr,
                 RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

static Settings GetSettingsSnapshot() {
    AcquireSRWLockShared(&g_settingsLock);
    Settings s = g_settings;
    ReleaseSRWLockShared(&g_settingsLock);
    return s;
}

static std::wstring ToLower(const std::wstring& s) {
    std::wstring r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::towlower);
    return r;
}

static bool NameMatches(const std::wstring& name, const Settings& s) {
    std::wstring low = ToLower(name);
    for (const auto& k : s.keywords) {
        if (!k.empty() && low.find(ToLower(k)) != std::wstring::npos) return true;
    }
    return false;
}

// ---- UIA pattern helpers ----

static void DoExpand(IUIAutomationElement* e) {
    IUIAutomationExpandCollapsePattern* p = nullptr;
    if (SUCCEEDED(e->GetCurrentPatternAs(
            UIA_ExpandCollapsePatternId,
            __uuidof(IUIAutomationExpandCollapsePattern), (void**)&p)) && p) {
        p->Expand();
        p->Release();
        return;
    }
    IUIAutomationInvokePattern* inv = nullptr;
    if (SUCCEEDED(e->GetCurrentPatternAs(
            UIA_InvokePatternId,
            __uuidof(IUIAutomationInvokePattern), (void**)&inv)) && inv) {
        inv->Invoke();
        inv->Release();
    }
}

static void DoCollapse(IUIAutomationElement* e) {
    IUIAutomationExpandCollapsePattern* p = nullptr;
    if (SUCCEEDED(e->GetCurrentPatternAs(
            UIA_ExpandCollapsePatternId,
            __uuidof(IUIAutomationExpandCollapsePattern), (void**)&p)) && p) {
        p->Collapse();
        p->Release();
        return;
    }
    // No ExpandCollapse support: a second Invoke toggles the flyout closed.
    IUIAutomationInvokePattern* inv = nullptr;
    if (SUCCEEDED(e->GetCurrentPatternAs(
            UIA_InvokePatternId,
            __uuidof(IUIAutomationInvokePattern), (void**)&inv)) && inv) {
        inv->Invoke();
        inv->Release();
    }
}

// ---- Locating the chevron button ----

static IUIAutomationElement* FindOverflowButton(IUIAutomation* pAuto,
                                                const Settings& s) {
    HWND hTaskbar = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (!hTaskbar) return nullptr;

    IUIAutomationElement* pRoot = nullptr;
    if (FAILED(pAuto->ElementFromHandle(hTaskbar, &pRoot)) || !pRoot) return nullptr;

    IUIAutomationCondition* pCond = nullptr;
    VARIANT v; VariantInit(&v);
    v.vt = VT_I4; v.lVal = UIA_ButtonControlTypeId;
    pAuto->CreatePropertyCondition(UIA_ControlTypePropertyId, v, &pCond);

    // Hybrid strategy:
    //  1) match by name (reliable for configured languages),
    //  2) fallback: the leftmost button with the configured AutomationId
    //     (language-independent but heuristic) — used only when no name matches.
    IUIAutomationElement* nameMatch = nullptr;
    IUIAutomationElement* leftmost = nullptr;
    LONG leftmostX = 0;

    IUIAutomationElementArray* pArr = nullptr;
    if (pCond && SUCCEEDED(pRoot->FindAll(TreeScope_Subtree, pCond, &pArr)) && pArr) {
        int n = 0; pArr->get_Length(&n);
        for (int i = 0; i < n; i++) {
            IUIAutomationElement* e = nullptr;
            if (FAILED(pArr->GetElement(i, &e)) || !e) continue;

            BSTR name = nullptr;
            e->get_CurrentName(&name);
            bool matched = (name && NameMatches(name, s));
            if (name) SysFreeString(name);
            if (matched) {
                nameMatch = e;           // keep the reference; a name match wins
                break;
            }

            // Fallback candidate: a tray button with the configured AutomationId.
            BSTR aid = nullptr;
            e->get_CurrentAutomationId(&aid);
            bool isTrayIcon = (aid && s.trayIconAutomationId == aid);
            if (aid) SysFreeString(aid);

            if (isTrayIcon) {
                RECT r;
                if (SUCCEEDED(e->get_CurrentBoundingRectangle(&r)) &&
                    (!leftmost || r.left < leftmostX)) {
                    if (leftmost) leftmost->Release();
                    leftmost = e;        // keep the new leftmost candidate
                    leftmostX = r.left;
                    e = nullptr;
                }
            }
            if (e) e->Release();
        }
        pArr->Release();
    }
    if (pCond) pCond->Release();
    pRoot->Release();

    if (nameMatch) {
        if (leftmost) leftmost->Release();
        return nameMatch;
    }
    return leftmost;   // nullptr if nothing matched
}

static bool PtInRectPad(const RECT& r, POINT pt, int pad) {
    return pt.x >= r.left - pad && pt.x <= r.right + pad &&
           pt.y >= r.top - pad  && pt.y <= r.bottom + pad;
}

static const ULONGLONG ACTION_COOLDOWN_MS = 300;
static const ULONGLONG REFIND_INTERVAL_MS = 3000;
static const ULONGLONG RECT_REFRESH_MS = 750;
static const ULONGLONG IDLE_STATE_CHECK_MS = 500;

// Cheap (pure Win32) resolution of the flyout window — no UIA calls. The
// chevron does not support the ExpandCollapse pattern anyway, so the flyout
// window's visibility is the only reliable state signal. Returns the window
// handle if the flyout is open, or nullptr otherwise. Resolving the handle
// once per tick lets callers reuse it instead of each calling FindWindowW.
static HWND GetVisibleFlyout(const Settings& s) {
    HWND f = FindWindowW(s.flyoutClass.c_str(), nullptr);
    return (f && IsWindowVisible(f)) ? f : nullptr;
}

static bool PtOverWindow(HWND hwnd, POINT pt) {
    if (!hwnd) return false;
    RECT r;
    if (!GetWindowRect(hwnd, &r)) return false;
    return PtInRect(&r, pt);
}

// Hide the chevron's "Hide" tooltip, which Windows pops up over the chevron
// while the flyout is open and can cover the bottom row of icons. The tooltip
// is a separate window class from the flyout, but that class is a generic XAML
// popup host, so only windows positioned over the chevron (horizontally
// overlapping it and sitting at or above its top, i.e. inside the flyout zone)
// are hidden — never popups elsewhere on screen.
static void HideChevronTooltip(const Settings& s, const RECT& chevron) {
    HWND h = nullptr;
    while ((h = FindWindowExW(nullptr, h, s.tooltipClass.c_str(), nullptr))) {
        if (!IsWindowVisible(h)) continue;
        RECT r;
        if (!GetWindowRect(h, &r)) continue;
        bool overlapsX = r.left <= chevron.right && r.right >= chevron.left;
        bool aboveChevron = r.bottom <= chevron.bottom;
        if (overlapsX && aboveChevron) {
            ShowWindow(h, SW_HIDE);
        }
    }
}

// True when the foreground window covers its whole monitor, i.e. a fullscreen
// app (a fullscreen video, a game, etc.). Maximized windows stop at the work
// area and therefore do not match. The desktop and shell windows are excluded
// so an empty desktop is not mistaken for a fullscreen app.
static bool IsForegroundFullscreen() {
    HWND hwnd = GetForegroundWindow();
    if (!hwnd || hwnd == GetShellWindow()) return false;

    WCHAR cls[64];
    if (GetClassNameW(hwnd, cls, ARRAYSIZE(cls))) {
        if (wcscmp(cls, L"Shell_TrayWnd") == 0 ||
            wcscmp(cls, L"Progman") == 0 ||
            wcscmp(cls, L"WorkerW") == 0) {
            return false;
        }
    }

    RECT wr;
    if (!GetWindowRect(hwnd, &wr)) return false;
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfoW(MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST), &mi)) {
        return false;
    }
    return wr.left <= mi.rcMonitor.left && wr.top <= mi.rcMonitor.top &&
           wr.right >= mi.rcMonitor.right && wr.bottom >= mi.rcMonitor.bottom;
}

// ---- Worker thread ----

static DWORD WINAPI WorkerThread(LPVOID) {
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    IUIAutomation* pAuto = nullptr;
    if (FAILED(CoCreateInstance(__uuidof(CUIAutomation), nullptr,
            CLSCTX_INPROC_SERVER, __uuidof(IUIAutomation), (void**)&pAuto)) || !pAuto) {
        Wh_Log(L"Failed to create IUIAutomation");
        CoUninitialize();
        return 0;
    }

    Settings s = GetSettingsSnapshot();
    int settingsGen = g_settingsGeneration;

    IUIAutomationElement* pBtn = nullptr;
    RECT cachedRect = {};
    bool haveRect = false;
    bool flyoutBelievedOpen = false;
    ULONGLONG leftAt = 0;
    ULONGLONG nextRefind = 0;
    ULONGLONG nextRectRefresh = 0;
    ULONGLONG nextIdleStateCheck = 0;
    ULONGLONG lastOpenAt = 0;
    bool overBtnPrev = false;
    bool clickedInFlyout = false;
    bool anyBtnDownPrev = false;

    while (g_running) {
        ULONGLONG now = GetTickCount64();

        if (g_settingsGeneration != settingsGen) {
            s = GetSettingsSnapshot();
            settingsGen = g_settingsGeneration;
            // Settings may change how the chevron is detected, so drop the
            // cached element and re-detect promptly with the new settings.
            if (pBtn) { pBtn->Release(); pBtn = nullptr; }
            haveRect = false;
            nextRefind = 0;
        }

        // Lazily (re-)find the button only when we don't have a valid one. A
        // destroyed or stale element makes the rectangle query below fail,
        // which clears pBtn and triggers a prompt re-find — so there is no need
        // for a periodic cross-process subtree walk while the element is valid.
        // When the chevron is absent (e.g. no hidden icons), throttle retries
        // to REFIND_INTERVAL_MS instead of walking the tree every tick.
        if (!pBtn && now >= nextRefind) {
            pBtn = FindOverflowButton(pAuto, s);
            nextRefind = now + REFIND_INTERVAL_MS;
            nextRectRefresh = 0;        // force a fresh rectangle below
        }

        if (pBtn) {
            // Expensive and RARE: query the button rectangle through UIA only
            // periodically, not every tick. The chevron rarely moves.
            if (now >= nextRectRefresh) {
                RECT r;
                if (SUCCEEDED(pBtn->get_CurrentBoundingRectangle(&r))) {
                    cachedRect = r;
                    haveRect = true;
                } else {
                    // The element is stale/destroyed (taskbar rebuilt). Drop it
                    // and re-find promptly on the next tick.
                    pBtn->Release(); pBtn = nullptr; haveRect = false;
                    nextRefind = 0;
                    WaitForSingleObject(g_stopEvent, s.pollInterval);
                    continue;
                }
                nextRectRefresh = now + RECT_REFRESH_MS;
            }
        }

        if (haveRect) {
            // Cheap and EVERY TICK: only local Win32 calls.
            POINT pt; GetCursorPos(&pt);
            bool overBtn = PtInRectPad(cachedRect, pt, s.pad);
            bool cooling = (now - lastOpenAt < ACTION_COOLDOWN_MS);
            bool enterEdge = overBtn && !overBtnPrev && !cooling;

            // The flyout state is only needed on the cursor-enter edge (to
            // avoid toggling an open flyout closed) and while auto-collapse is
            // watching an open flyout. When idle, throttle the check so a
            // manually opened flyout is still noticed without paying a
            // FindWindowW call on every tick.
            HWND flyoutHwnd = nullptr;
            bool wantState = enterEdge
                          || (s.autoClose && flyoutBelievedOpen)
                          || (s.hideTooltip && overBtn);
            if (wantState) {
                flyoutHwnd = GetVisibleFlyout(s);
            } else if (s.autoClose && now >= nextIdleStateCheck) {
                flyoutHwnd = GetVisibleFlyout(s);
                nextIdleStateCheck = now + IDLE_STATE_CHECK_MS;
            }
            bool flyoutVisible = (flyoutHwnd != nullptr);
            flyoutBelievedOpen = flyoutVisible || cooling;

            // While hovering the chevron of an open flyout, suppress the
            // "Hide" tooltip that would otherwise cover the bottom icons.
            if (s.hideTooltip && overBtn && flyoutVisible) {
                HideChevronTooltip(s, cachedRect);
            }

            // Open only on the cursor-enter edge and only when the flyout is
            // not already open. The chevron acts as a toggle, so any redundant
            // Invoke would close it again. Skip activation over a fullscreen
            // app so the flyout can't pop up on top of a video or a game (the
            // taskbar is hidden there, but the cached chevron rect still matches
            // that screen area). Checked only on the edge, so it costs nothing
            // on idle ticks.
            if (enterEdge && !flyoutVisible &&
                !(s.suppressInFullscreen && IsForegroundFullscreen())) {
                DoExpand(pBtn);
                lastOpenAt = now;
                flyoutBelievedOpen = true;
                leftAt = 0;
                Wh_Log(L"OPEN");
            }
            overBtnPrev = overBtn;

            // Detect a click inside the flyout: the user is interacting with
            // an icon (e.g. its popup window just opened and took focus).
            // Suspend auto-collapse until the flyout closes on its own —
            // collapsing via Invoke would steal focus and dismiss the popup
            // the user just opened.
            bool anyBtnDown = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ||
                              (GetAsyncKeyState(VK_RBUTTON) & 0x8000) ||
                              (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
            if (anyBtnDown && !anyBtnDownPrev && flyoutVisible &&
                PtOverWindow(flyoutHwnd, pt)) {
                clickedInFlyout = true;
                Wh_Log(L"Click inside flyout: auto-collapse suspended");
            }
            anyBtnDownPrev = anyBtnDown;
            if (!flyoutVisible && !cooling) {
                clickedInFlyout = false;
            }

            // Auto-collapse once the cursor left both the button and the flyout.
            if (s.autoClose && flyoutVisible && !cooling && !clickedInFlyout) {
                bool overFlyout = PtOverWindow(flyoutHwnd, pt);
                if (overBtn || overFlyout) {
                    leftAt = 0;
                } else if (leftAt == 0) {
                    leftAt = now;
                } else if (now - leftAt >= (ULONGLONG)s.grace) {
                    DoCollapse(pBtn);
                    leftAt = 0;
                    flyoutBelievedOpen = false;
                    Wh_Log(L"CLOSE after grace");
                }
            } else {
                leftAt = 0;
            }
        }

        // Interruptible sleep: WhTool_ModUninit signals g_stopEvent so the
        // thread wakes immediately regardless of the polling interval.
        WaitForSingleObject(g_stopEvent, s.pollInterval);
    }

    if (pBtn) pBtn->Release();
    if (pAuto) pAuto->Release();
    RestoreFlyoutAppearance();
    CoUninitialize();
    return 0;
}

// ---- Explorer-side XAML background styling ----

namespace PopupStyle {

namespace Xaml = winrt::Windows::UI::Xaml;
namespace Controls = winrt::Windows::UI::Xaml::Controls;
namespace Media = winrt::Windows::UI::Xaml::Media;

struct StyledBorder {
    winrt::weak_ref<Controls::Border> border;
    Media::Brush originalBackground{nullptr};
};

static std::vector<StyledBorder> g_styledBorders;
static std::atomic<bool> g_unloading{false};
static std::atomic<bool> g_symbolsHooked{false};
static int g_tintOpacity = 28;
static int g_luminosityOpacity = 55;

using FrameworkElementLoadedEventRevoker = winrt::impl::event_revoker<
    Xaml::IFrameworkElement,
    &winrt::impl::abi<Xaml::IFrameworkElement>::type::remove_Loaded>;

static std::list<FrameworkElementLoadedEventRevoker> g_loadedRevokers;

static void LoadStyleSettings() {
    g_tintOpacity = std::clamp(
        static_cast<int>(Wh_GetIntSetting(L"backgroundTintOpacity")), 0, 100);
    g_luminosityOpacity = std::clamp(
        static_cast<int>(Wh_GetIntSetting(L"backgroundLuminosityOpacity")),
        0, 100);
}

static Media::AcrylicBrush CreateAcrylicBrush() {
    Media::AcrylicBrush brush;
    brush.BackgroundSource(Media::AcrylicBackgroundSource::HostBackdrop);
    brush.TintColor(winrt::Windows::UI::Color{255, 0, 0, 0});
    brush.FallbackColor(winrt::Windows::UI::Color{235, 18, 18, 18});
    brush.TintOpacity(g_tintOpacity / 100.0);
    brush.TintLuminosityOpacity(g_luminosityOpacity / 100.0);
    brush.Opacity(1.0);
    return brush;
}

static Xaml::FrameworkElement FindDescendantByName(
    Xaml::FrameworkElement parent, const wchar_t* name) {
    if (!parent) return nullptr;

    int count = Media::VisualTreeHelper::GetChildrenCount(parent);
    for (int i = 0; i < count; i++) {
        auto child = Media::VisualTreeHelper::GetChild(parent, i)
                         .try_as<Xaml::FrameworkElement>();
        if (!child) continue;
        if (child.Name() == name) return child;
        if (auto found = FindDescendantByName(child, name)) return found;
    }

    return nullptr;
}

static Xaml::FrameworkElement FindOverflowAncestor(
    Xaml::FrameworkElement element) {
    auto current = element;
    while (current) {
        if (winrt::get_class_name(current) ==
            L"SystemTray.NotificationAreaOverflow") {
            return current;
        }
        current = Media::VisualTreeHelper::GetParent(current)
                      .try_as<Xaml::FrameworkElement>();
    }
    return nullptr;
}

static bool IsStored(Controls::Border border) {
    for (auto it = g_styledBorders.begin(); it != g_styledBorders.end();) {
        auto stored = it->border.get();
        if (!stored) {
            it = g_styledBorders.erase(it);
            continue;
        }
        if (winrt::get_abi(stored) == winrt::get_abi(border)) return true;
        ++it;
    }
    return false;
}

static void StyleFromIcon(Xaml::FrameworkElement icon) {
    if (g_unloading) return;

    auto overflow = FindOverflowAncestor(icon);
    if (!overflow) return;

    auto border = FindDescendantByName(
                      overflow, L"OverflowFlyoutBackgroundBorder")
                      .try_as<Controls::Border>();
    if (!border) {
        Wh_Log(L"OverflowFlyoutBackgroundBorder wasn't found");
        return;
    }

    if (!IsStored(border)) {
        StyledBorder stored;
        stored.border = border;
        stored.originalBackground = border.Background();
        g_styledBorders.push_back(stored);
    }

    border.Background(CreateAcrylicBrush());
    Wh_Log(L"Applied real acrylic system-tray background");
}

static void ReapplyAll() {
    for (auto it = g_styledBorders.begin(); it != g_styledBorders.end();) {
        auto border = it->border.get();
        if (!border) {
            it = g_styledBorders.erase(it);
            continue;
        }
        border.Background(CreateAcrylicBrush());
        ++it;
    }
}

static void RestoreAll() {
    for (auto& stored : g_styledBorders) {
        if (auto border = stored.border.get()) {
            border.Background(stored.originalBackground);
        }
    }
    g_styledBorders.clear();
    g_loadedRevokers.clear();
}

using IconViewConstructor_t = void*(WINAPI*)(void*);
static IconViewConstructor_t IconViewConstructor_Original;

static void* WINAPI IconViewConstructor_Hook(void* pThis) {
    void* result = IconViewConstructor_Original(pThis);
    if (g_unloading) return result;

    try {
        Xaml::FrameworkElement icon = nullptr;
        reinterpret_cast<IUnknown**>(pThis)[1]->QueryInterface(
            winrt::guid_of<Xaml::FrameworkElement>(), winrt::put_abi(icon));
        if (!icon) return result;

        g_loadedRevokers.emplace_back();
        auto revoker = std::prev(g_loadedRevokers.end());
        *revoker = icon.Loaded(
            winrt::auto_revoke_t{},
            [revoker](winrt::Windows::Foundation::IInspectable const& sender,
                      Xaml::RoutedEventArgs const&) {
                g_loadedRevokers.erase(revoker);
                if (g_unloading) return;
                if (auto element = sender.try_as<Xaml::FrameworkElement>()) {
                    StyleFromIcon(element);
                }
            });
    } catch (...) {
        Wh_Log(L"Popup style error: 0x%08X", winrt::to_hresult());
    }

    return result;
}

static bool HookSystemTrayModule(HMODULE module) {
    if (!module || g_symbolsHooked.exchange(true)) return true;

    WindhawkUtils::SYMBOL_HOOK hooks[] = {{
        {LR"(public: __cdecl winrt::SystemTray::implementation::IconView::IconView(void))"},
        &IconViewConstructor_Original,
        IconViewConstructor_Hook,
    }};

    if (!HookSymbols(module, hooks, ARRAYSIZE(hooks))) {
        g_symbolsHooked = false;
        Wh_Log(L"Failed to hook the SystemTray IconView constructor");
        return false;
    }

    return true;
}

using LoadLibraryExW_t = decltype(&LoadLibraryExW);
static LoadLibraryExW_t LoadLibraryExW_Original;

static HMODULE WINAPI LoadLibraryExW_Hook(LPCWSTR fileName, HANDLE file,
                                          DWORD flags) {
    HMODULE module = LoadLibraryExW_Original(fileName, file, flags);
    if (module && !g_symbolsHooked && fileName) {
        const wchar_t* baseName = wcsrchr(fileName, L'\\');
        baseName = baseName ? baseName + 1 : fileName;
        if (_wcsicmp(baseName, L"SystemTray.dll") == 0 ||
            _wcsicmp(baseName, L"Taskbar.View.dll") == 0 ||
            _wcsicmp(baseName, L"ExplorerExtensions.dll") == 0) {
            if (HookSystemTrayModule(module)) Wh_ApplyHookOperations();
        }
    }
    return module;
}

static HMODULE FindSystemTrayModule() {
    if (HMODULE module = GetModuleHandleW(L"SystemTray.dll")) return module;
    if (HMODULE module = GetModuleHandleW(L"Taskbar.View.dll")) return module;
    return GetModuleHandleW(L"ExplorerExtensions.dll");
}

using WindowThreadProc = void(WINAPI*)(void*);

static bool RunOnWindowThread(HWND window, WindowThreadProc proc) {
    if (!window) return false;
    static UINT message = RegisterWindowMessageW(
        L"Windhawk_TrayHoverExpand_AcrylicUIThread");

    struct Params {
        WindowThreadProc proc;
    } params{proc};

    DWORD threadId = GetWindowThreadProcessId(window, nullptr);
    if (!threadId) return false;
    if (threadId == GetCurrentThreadId()) {
        proc(nullptr);
        return true;
    }

    HHOOK hook = SetWindowsHookExW(
        WH_CALLWNDPROC,
        [](int code, WPARAM wParam, LPARAM lParam) -> LRESULT {
            if (code == HC_ACTION) {
                auto messageData = reinterpret_cast<CWPSTRUCT*>(lParam);
                if (messageData->message == message) {
                    reinterpret_cast<Params*>(messageData->lParam)->proc(nullptr);
                }
            }
            return CallNextHookEx(nullptr, code, wParam, lParam);
        },
        nullptr, threadId);
    if (!hook) return false;

    SendMessageW(window, message, 0, reinterpret_cast<LPARAM>(&params));
    UnhookWindowsHookEx(hook);
    return true;
}

static HWND FindXamlWindow() {
    if (HWND overflow =
            FindWindowW(L"TopLevelWindowForOverflowXamlIsland", nullptr)) {
        return overflow;
    }
    HWND taskbar = FindWindowW(L"Shell_TrayWnd", nullptr);
    return taskbar
               ? FindWindowExW(
                     taskbar, nullptr,
                     L"Windows.UI.Composition.DesktopWindowContentBridge",
                     nullptr)
               : nullptr;
}

static BOOL Init() {
    g_unloading = false;
    LoadStyleSettings();

    if (HMODULE module = FindSystemTrayModule()) {
        return HookSystemTrayModule(module);
    }

    HMODULE kernelBase = GetModuleHandleW(L"kernelbase.dll");
    auto loadLibrary = reinterpret_cast<LoadLibraryExW_t>(
        GetProcAddress(kernelBase, "LoadLibraryExW"));
    return loadLibrary &&
           Wh_SetFunctionHook(reinterpret_cast<void*>(loadLibrary),
                              reinterpret_cast<void*>(LoadLibraryExW_Hook),
                              reinterpret_cast<void**>(&LoadLibraryExW_Original));
}

static void SettingsChanged() {
    LoadStyleSettings();
    RunOnWindowThread(FindXamlWindow(), [](void*) { ReapplyAll(); });
}

static void Uninit() {
    g_unloading = true;
    RunOnWindowThread(FindXamlWindow(), [](void*) { RestoreAll(); });
}

}  // namespace PopupStyle

// ---- Mod lifecycle (tool mod) ----

static void LoadSettings() {
    Settings s;

    s.autoClose = Wh_GetIntSetting(L"autoClose") != 0;
    s.pollInterval = (int)Wh_GetIntSetting(L"pollInterval");
    s.grace = (int)Wh_GetIntSetting(L"grace");
    s.pad = (int)Wh_GetIntSetting(L"pad");
    s.hideTooltip = Wh_GetIntSetting(L"hideTooltip") != 0;
    s.suppressInFullscreen = Wh_GetIntSetting(L"suppressInFullscreen") != 0;
    if (s.pollInterval < 10) s.pollInterval = 10;
    if (s.grace < 0) s.grace = 0;

    // Wh_GetStringSetting never returns NULL (it returns L"" on unset/error),
    // so only override the defaults with non-empty values.
    PCWSTR fc = Wh_GetStringSetting(L"flyoutClass");
    if (*fc) s.flyoutClass = fc;
    Wh_FreeStringSetting(fc);

    PCWSTR tc = Wh_GetStringSetting(L"tooltipClass");
    if (*tc) s.tooltipClass = tc;
    Wh_FreeStringSetting(tc);

    PCWSTR aid = Wh_GetStringSetting(L"trayIconAutomationId");
    if (*aid) s.trayIconAutomationId = aid;
    Wh_FreeStringSetting(aid);

    std::vector<std::wstring> keywords;
    for (int i = 0;; i++) {
        PCWSTR k = Wh_GetStringSetting(L"keywords[%d]", i);
        bool empty = !*k;
        if (!empty) keywords.push_back(k);
        Wh_FreeStringSetting(k);
        if (empty) break;
    }
    if (!keywords.empty()) s.keywords = std::move(keywords);

    AcquireSRWLockExclusive(&g_settingsLock);
    g_settings = std::move(s);
    ReleaseSRWLockExclusive(&g_settingsLock);
    g_settingsGeneration++;
}

BOOL WhTool_ModInit() {
    LoadSettings();

    g_stopEvent = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    if (!g_stopEvent) {
        Wh_Log(L"CreateEvent failed");
        return FALSE;
    }

    g_running = true;
    g_thread = CreateThread(nullptr, 0, WorkerThread, nullptr, 0, nullptr);
    if (!g_thread) {
        g_running = false;
        CloseHandle(g_stopEvent);
        g_stopEvent = nullptr;
        return FALSE;
    }
    return TRUE;
}

void WhTool_ModSettingsChanged() {
    LoadSettings();
}

void WhTool_ModUninit() {
    g_running = false;
    if (g_stopEvent) {
        SetEvent(g_stopEvent);
    }
    if (g_thread) {
        // Safe to wait without a timeout: the worker only blocks in the
        // interruptible wait above, so it exits promptly once signaled.
        WaitForSingleObject(g_thread, INFINITE);
        CloseHandle(g_thread);
        g_thread = nullptr;
    }
    if (g_stopEvent) {
        CloseHandle(g_stopEvent);
        g_stopEvent = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Windhawk tool mod implementation for mods which don't need to inject to other
// processes or hook other functions. Context:
// https://github.com/ramensoftware/windhawk/wiki/Mods-as-tools:-Running-mods-in-a-dedicated-process
//
// The mod will load and run in a dedicated windhawk.exe process.
//
// Paste the code below as part of the mod code, and use these callbacks:
// * WhTool_ModInit
// * WhTool_ModSettingsChanged
// * WhTool_ModUninit
//
// Currently, other callbacks are not supported.

bool g_isToolModProcessLauncher;
bool g_isExplorerStyleProcess;
HANDLE g_toolModProcessMutex;

void WINAPI EntryPoint_Hook() {
    Wh_Log(L">");
    ExitThread(0);
}

BOOL Wh_ModInit() {
    WCHAR processPath[MAX_PATH];
    if (GetModuleFileNameW(nullptr, processPath, ARRAYSIZE(processPath))) {
        const wchar_t* processName = wcsrchr(processPath, L'\\');
        processName = processName ? processName + 1 : processPath;
        if (_wcsicmp(processName, L"explorer.exe") == 0) {
            g_isExplorerStyleProcess = true;
            Wh_Log(L"Loading tray hover worker and acrylic background in Explorer");

            if (!PopupStyle::Init()) {
                Wh_Log(L"Failed to initialize the acrylic background");
                return FALSE;
            }

            if (!WhTool_ModInit()) {
                Wh_Log(L"Failed to initialize the tray hover worker");
                PopupStyle::Uninit();
                return FALSE;
            }

            return TRUE;
        }
    }

    DWORD sessionId;
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId) &&
        sessionId == 0) {
        return FALSE;
    }

    bool isExcluded = false;
    bool isToolModProcess = false;
    bool isCurrentToolModProcess = false;
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);
    if (!argv) {
        Wh_Log(L"CommandLineToArgvW failed");
        return FALSE;
    }

    for (int i = 1; i < argc; i++) {
        if (wcscmp(argv[i], L"-service") == 0 ||
            wcscmp(argv[i], L"-service-start") == 0 ||
            wcscmp(argv[i], L"-service-stop") == 0) {
            isExcluded = true;
            break;
        }
    }

    for (int i = 1; i < argc - 1; i++) {
        if (wcscmp(argv[i], L"-tool-mod") == 0) {
            isToolModProcess = true;
            if (wcscmp(argv[i + 1], WH_MOD_ID) == 0) {
                isCurrentToolModProcess = true;
            }
            break;
        }
    }

    LocalFree(argv);

    if (isExcluded) {
        return FALSE;
    }

    if (isCurrentToolModProcess) {
        g_toolModProcessMutex =
            CreateMutex(nullptr, TRUE, L"windhawk-tool-mod_" WH_MOD_ID);
        if (!g_toolModProcessMutex) {
            Wh_Log(L"CreateMutex failed");
            ExitProcess(1);
        }

        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            Wh_Log(L"Tool mod already running (%s)", WH_MOD_ID);
            ExitProcess(1);
        }

        if (!WhTool_ModInit()) {
            ExitProcess(1);
        }

        IMAGE_DOS_HEADER* dosHeader =
            (IMAGE_DOS_HEADER*)GetModuleHandle(nullptr);
        IMAGE_NT_HEADERS* ntHeaders =
            (IMAGE_NT_HEADERS*)((BYTE*)dosHeader + dosHeader->e_lfanew);

        DWORD entryPointRVA = ntHeaders->OptionalHeader.AddressOfEntryPoint;
        void* entryPoint = (BYTE*)dosHeader + entryPointRVA;

        Wh_SetFunctionHook(entryPoint, (void*)EntryPoint_Hook, nullptr);
        return TRUE;
    }

    if (isToolModProcess) {
        return FALSE;
    }

    g_isToolModProcessLauncher = true;
    return TRUE;
}

void Wh_ModAfterInit() {
    if (g_isExplorerStyleProcess) {
        return;
    }

    if (!g_isToolModProcessLauncher) {
        return;
    }

    WCHAR currentProcessPath[MAX_PATH];
    switch (GetModuleFileName(nullptr, currentProcessPath,
                              ARRAYSIZE(currentProcessPath))) {
        case 0:
        case ARRAYSIZE(currentProcessPath):
            Wh_Log(L"GetModuleFileName failed");
            return;
    }

    WCHAR commandLine[MAX_PATH + 2 +
                (sizeof(L" -tool-mod \"" WH_MOD_ID "\"") / sizeof(WCHAR)) - 1];
    swprintf_s(commandLine, L"\"%s\" -tool-mod \"%s\"", currentProcessPath,
               WH_MOD_ID);

    HMODULE kernelModule = GetModuleHandle(L"kernelbase.dll");
    if (!kernelModule) {
        kernelModule = GetModuleHandle(L"kernel32.dll");
        if (!kernelModule) {
            Wh_Log(L"No kernelbase.dll/kernel32.dll");
            return;
        }
    }

    using CreateProcessInternalW_t = BOOL(WINAPI*)(
        HANDLE, LPCWSTR, LPWSTR, LPSECURITY_ATTRIBUTES,
        LPSECURITY_ATTRIBUTES, WINBOOL, DWORD, LPVOID, LPCWSTR,
        LPSTARTUPINFOW, LPPROCESS_INFORMATION, PHANDLE);
    CreateProcessInternalW_t pCreateProcessInternalW =
        (CreateProcessInternalW_t)GetProcAddress(kernelModule,
                                                 "CreateProcessInternalW");
    if (!pCreateProcessInternalW) {
        Wh_Log(L"No CreateProcessInternalW");
        return;
    }

    STARTUPINFO si{
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_FORCEOFFFEEDBACK,
    };
    PROCESS_INFORMATION pi;
    if (!pCreateProcessInternalW(nullptr, currentProcessPath, commandLine,
                                 nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS,
                                 nullptr, nullptr, &si, &pi, nullptr)) {
        Wh_Log(L"CreateProcess failed");
        return;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void Wh_ModSettingsChanged() {
    if (g_isExplorerStyleProcess) {
        WhTool_ModSettingsChanged();
        PopupStyle::SettingsChanged();
        return;
    }

    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModSettingsChanged();
}

void Wh_ModUninit() {
    if (g_isExplorerStyleProcess) {
        WhTool_ModUninit();
        PopupStyle::Uninit();
        return;
    }

    if (g_isToolModProcessLauncher) {
        return;
    }

    WhTool_ModUninit();
    ExitProcess(0);
}
