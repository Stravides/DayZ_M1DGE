# TraderX - Complete Installation and Configuration Guide

**Version:** 1.0.1

---

## Table of Contents

1. [Installation](#1-installation)
2. [Folder Structure](#2-folder-structure)
3. [Configuration Format Selection (LEGACY / CSV / COMPILED)](#3-configuration-format-selection)
4. [General Settings (TraderXGeneralSettings.json)](#4-general-settings)
5. [Currency Configuration (TraderXCurrencySettings.json)](#5-currency-configuration)
6. [Products Configuration](#6-products-configuration)
7. [Categories Configuration](#7-categories-configuration)
8. [NPC Traders — How to Create and Configure](#8-npc-traders)
9. [Vehicle Parking](#9-vehicle-parking)
10. [Server Presets](#10-server-presets)
11. [Licenses](#11-licenses)
12. [Dynamic Translations (TraderXDynamicTranslationSettings.json)](#12-dynamic-translations)
13. [Debug Settings (TraderXDebugSettings.json)](#13-debug-settings)
14. [CSV Format Reference](#14-csv-format-reference)
15. [Migration Guide (Legacy → CSV → Compiled)](#15-migration-guide)
16. [FAQ — Common Questions](#16-faq)

---

## 1. Installation

### Requirements
- DayZ Standalone Server
- **CF (Community Framework)** mod
- **DabsFramework** mod

### Steps
1. Download the TraderX mod
2. Place the `@TraderX` folder in your server's mod directory
3. Add `@TraderX` to your server's `-mod=` launch parameter (along with `@CF` and `@DabsFramework`)
4. Start the server **once** — all default configuration files will be auto-generated under `$profile/TraderX/`
5. **Stop the server** and edit configuration files as needed (see sections below)
6. Restart the server

### Load Order
Ensure your `-mod=` parameter loads dependencies **before** TraderX:
```
-mod=@CF;@DabsFramework;@TraderX
```

> **Note:** `$profile` is your server's profile directory. If you launch with `-profiles=myProfile`, then files are in `myProfile/TraderX/`. If you don't specify `-profiles`, it defaults to the server root directory.

---

## 2. Folder Structure

On first startup, TraderX auto-creates the following structure under `$profile`:

```
TraderX/
├── TraderXConfig/
│   ├── TraderXGeneralSettings.json         # NPCs, objects, licenses, item condition rules
│   ├── TraderXCurrencySettings.json        # Currency types and denominations
│   ├── TraderXDynamicTranslationSettings.json  # All UI messages (customizable)
│   ├── TraderXDebugSettings.json           # Debug mode and test suite config
│   ├── SourceConfig.json                   # Choose LEGACY / CSV / COMPILED format
│   │
│   ├── Products/                           # [LEGACY format] One JSON file per product
│   │   └── <uuid>.json
│   ├── Categories/                         # [LEGACY format] One JSON file per category
│   │   └── <uuid>.json
│   │
│   ├── Source/                             # [CSV format] Editable spreadsheet files
│   │   ├── products.csv                    #   (or multiple: 10_weapons.csv, 20_vehicles.csv)
│   │   └── categories.csv
│   │
│   ├── Compiled/                           # [COMPILED format] Auto-generated, optimized
│   │   ├── Products_compiled.json
│   │   └── Categories_compiled.json
│   ├── Backup/                             # Auto-backup before each recompile
│   │
│   └── TraderXVehicleParking/              # Vehicle spawn/parking positions per trader
│       ├── parking_config.json             #   Whitelist of objects that don't block parking
│       └── parkings_<npcId>.json           #   Parking slots for a specific trader NPC
│
├── TraderXDatabase/                        # Runtime stock data (DO NOT EDIT while running)
├── TraderXPresets/                         # Player-created weapon presets (auto-managed)
│   └── presets_<productId>.json
├── TraderXFavorites/                       # Player favorites (auto-managed)
│   └── Fav_<steamId>.json
├── TraderXLicenses/                        # Player license ownership (auto-managed)
│   └── playerLicense_<steamId>.json
└── TraderXLogs/
    ├── Config/
    │   ├── LoggingSettings.json
    │   └── ConfigReport.log                # Validation report after CSV compile
    └── Logs/
        └── TraderX_<date>.log
```

### Which files do I edit?

| I want to... | Edit this file |
|---|---|
| Add/remove traders, change positions | `TraderXGeneralSettings.json` |
| Add/change currencies | `TraderXCurrencySettings.json` |
| Add/edit products (Legacy) | `Products/<uuid>.json` |
| Add/edit products (CSV) | `Source/products.csv` |
| Add/edit categories (Legacy) | `Categories/<uuid>.json` |
| Add/edit categories (CSV) | `Source/categories.csv` |
| Change format (Legacy/CSV/Compiled) | `SourceConfig.json` |
| Configure vehicle parking spots | `TraderXVehicleParking/parkings_<npcId>.json` |
| Customize UI messages | `TraderXDynamicTranslationSettings.json` |
| Enable debug mode | `TraderXDebugSettings.json` |

---

## 3. Configuration Format Selection

TraderX supports **three formats** for products and categories. You choose via `SourceConfig.json`.

### File: `TraderXConfig/SourceConfig.json`

```json
{
    "preferredFormat": "LEGACY",
    "autoMigrate": false,
    "allowCompilation": true
}
```

| Field | Type | Default | Description |
|---|---|---|---|
| `preferredFormat` | string | `"LEGACY"` | `"LEGACY"`, `"CSV"`, or `"COMPILED"` |
| `autoMigrate` | bool | `false` | If `true`, auto-converts legacy files to the chosen format on first run |
| `allowCompilation` | bool | `true` | If `true`, CSV files are compiled to optimized JSON on each server start |

### The Three Formats

**LEGACY** — One JSON file per product/category (default, simplest)
- Files: `Products/<uuid>.json`, `Categories/<uuid>.json`
- Edit individual files directly
- Best for: small servers, beginners, manual editing

**CSV** — Spreadsheet files (tab-delimited)
- Files: `Source/products.csv`, `Source/categories.csv`
- Edit in Excel, Google Sheets, or any text editor
- Auto-compiled to `Compiled/` on each server start
- Supports **multiple CSV packs** (e.g., `10_weapons.csv`, `20_vehicles.csv`) merged alphabetically by filename prefix
- Best for: medium/large servers, bulk editing

**COMPILED** — Single optimized JSON per type
- Files: `Compiled/Products_compiled.json`, `Compiled/Categories_compiled.json`
- Either auto-generated from CSV, or edited directly
- Fastest loading
- Best for: performance-focused servers

### Loading Priority

On server start, TraderX loads in this order:
1. **Compiled JSON** (if exists and `preferredFormat` is `"COMPILED"`)
2. **CSV Source** → compile → load (if `preferredFormat` is `"CSV"`)
3. **Legacy JSON** (if `preferredFormat` is `"LEGACY"`)
4. If nothing found → create empty defaults

### Quick Setup Examples

**I just want it to work (Legacy — default):**
```json
{ "preferredFormat": "LEGACY", "autoMigrate": false, "allowCompilation": true }
```

**I want to use spreadsheets (CSV):**
```json
{ "preferredFormat": "CSV", "autoMigrate": true, "allowCompilation": true }
```
Set `autoMigrate: true` the first time to convert your existing legacy files to CSV. After the first run, set it back to `false`.

**I want maximum performance (Compiled):**
```json
{ "preferredFormat": "COMPILED", "autoMigrate": true, "allowCompilation": true }
```

---

## 4. General Settings

### File: `TraderXConfig/TraderXGeneralSettings.json`

This is the **main configuration file**. It defines traders (NPCs), world objects, licenses, and item condition rules.

```json
{
    "version": "1.0.1",
    "serverID": "auto-generated-uuid",
    "licenses": [
        {
            "licenseId": "auto-generated",
            "licenseName": "Car License",
            "description": "Allows you to drive a car."
        }
    ],
    "acceptedStates": {
        "acceptWorn": true,
        "acceptDamaged": true,
        "acceptBadlyDamaged": true,
        "coefficientWorn": 0.75,
        "coefficientDamaged": 0.5,
        "coefficientBadlyDamaged": 0.25
    },
    "traders": [ ... ],
    "traderObjects": [ ... ]
}
```

### Field Reference

| Field | Type | Description |
|---|---|---|
| `version` | string | Auto-managed version string. Do not edit. |
| `serverID` | string | Unique server identifier (auto-generated on first run). |
| `licenses` | array | List of licenses that can be required to access categories. |
| `acceptedStates` | object | Rules for buying/selling items based on condition. |
| `traders` | array | List of NPC trader definitions (see [Section 8](#8-npc-traders)). |
| `traderObjects` | array | World objects spawned around traders (decorations, barriers). |

### acceptedStates — Item Condition Rules

Controls whether the trader accepts worn/damaged items and at what price reduction.

| Field | Type | Default | Description |
|---|---|---|---|
| `acceptWorn` | bool | `true` | Accept items in "Worn" condition |
| `acceptDamaged` | bool | `true` | Accept items in "Damaged" condition |
| `acceptBadlyDamaged` | bool | `true` | Accept items in "Badly Damaged" condition |
| `coefficientWorn` | float | `0.75` | Sell price multiplier for worn items (75%) |
| `coefficientDamaged` | float | `0.5` | Sell price multiplier for damaged items (50%) |
| `coefficientBadlyDamaged` | float | `0.25` | Sell price multiplier for badly damaged items (25%) |

### traderObjects — World Decorations

Each entry spawns a static object in the world (barriers, buildings, etc.):

```json
{
    "className": "bldr_Wall_Barricade1_10",
    "position": [11590.55, 58.22, 14760.67],
    "orientation": [144, 0, 0]
}
```

| Field | Type | Description |
|---|---|---|
| `className` | string | DayZ class name of the object to spawn |
| `position` | vector | World position `[x, y, z]` |
| `orientation` | vector | Rotation `[yaw, pitch, roll]` in degrees |

---

## 5. Currency Configuration

### File: `TraderXConfig/TraderXCurrencySettings.json`

Defines all currency types and their denominations. Each NPC trader can accept one or more currency types.

```json
{
    "version": "1.0.1",
    "currencyTypes": [
        {
            "currencyName": "EUR",
            "currencies": [
                { "className": "TraderX_Money_Euro100", "value": 100 },
                { "className": "TraderX_Money_Euro50",  "value": 50 },
                { "className": "TraderX_Money_Euro10",  "value": 10 },
                { "className": "TraderX_Money_Euro5",   "value": 5 },
                { "className": "TraderX_Money_Euro1",   "value": 1 }
            ]
        },
        {
            "currencyName": "USD",
            "currencies": [
                { "className": "TraderX_Money_Dollar100", "value": 100 },
                { "className": "TraderX_Money_Dollar50",  "value": 50 },
                { "className": "TraderX_Money_Dollar10",  "value": 10 },
                { "className": "TraderX_Money_Dollar5",   "value": 5 },
                { "className": "TraderX_Money_Dollar1",   "value": 1 }
            ]
        }
    ]
}
```

### How to add a new currency

1. Add a new object to the `currencyTypes` array
2. Set `currencyName` to a unique name (e.g., `"GBP"`, `"Gold"`, `"Rubles"`)
3. Add denominations in the `currencies` array — each needs a DayZ `className` and an integer `value`
4. The `className` must be a valid item class that exists in the game or a mod
5. Assign this currency to a trader by adding the `currencyName` to the trader's `currenciesAccepted` array in `TraderXGeneralSettings.json`

**Example — Adding a Gold currency:**
```json
{
    "currencyName": "Gold",
    "currencies": [
        { "className": "GoldBar",    "value": 1000 },
        { "className": "GoldNugget", "value": 100 },
        { "className": "GoldCoin",   "value": 10 }
    ]
}
```

> **Important:** Denominations are sorted highest-to-lowest automatically. The system uses the largest denomination first when giving change.

---

## 6. Products Configuration

A **product** is any item that can be bought or sold at a trader. Each product has a price, stock settings, and optional attachments/variants.

### Legacy Format — `Products/<uuid>.json`

Each product is a separate JSON file named with a UUID (auto-generated).

```json
{
    "className": "AKM",
    "coefficient": 1.0,
    "maxStock": 10,
    "tradeQuantity": 0,
    "buyPrice": 5000,
    "sellPrice": 2500,
    "stockSettings": 0,
    "attachments": [
        "uuid-of-akm-magazine",
        "uuid-of-akm-buttstock"
    ],
    "variants": [
        "uuid-of-akm-variant"
    ],
    "productId": "auto-generated-uuid"
}
```

### Product Field Reference

| Field | Type | Default | Description |
|---|---|---|---|
| `className` | string | — | **DayZ class name** of the item (e.g., `"AKM"`, `"OffroadHatchback"`) |
| `buyPrice` | int | `-1` | Price to buy. Set to `-1` to **disable buying** |
| `sellPrice` | int | `-1` | Price when selling. Set to `-1` to **disable selling** |
| `maxStock` | int | `-1` | Maximum stock. `-1` = **unlimited stock** |
| `coefficient` | float | `1.0` | Legacy destock coefficient (see `stockSettings` for new system) |
| `tradeQuantity` | int | `0` | Bitfield controlling buy/sell quantity modes (see below) |
| `stockSettings` | int | `0` | Bitfield: destock coefficient + stock behavior at restart |
| `attachments` | array | `[]` | List of product UUIDs (or productKeys in CSV) for attachments |
| `variants` | array | `[]` | List of product UUIDs (or productKeys in CSV) for variants |
| `productId` | string | auto | Unique identifier. Auto-generated, do not edit manually |

### Stock Settings Explained

The `stockSettings` field is a **bitfield** that packs two values:

- **Destock Coefficient** (bits 0-6): How fast stock decreases over time. Value 0-100 representing 0.00 to 1.00. `0` = no destock.
- **Stock Behavior at Restart** (bits 7-8): What happens to stock when the server restarts.
  - `0` = **Keep** current stock (default)
  - `1` = **Reset** to `maxStock`
  - `2` = **Random** stock between 0 and `maxStock`

> In CSV format, these are separate human-readable columns (`destockCoefficient`, `stockBehaviorAtRestart`).

### Trade Quantity Modes

The `tradeQuantity` bitfield controls how items are filled when bought/sold:

**Sell Modes** (how full must the item be to sell):
| Mode | Value | Description |
|---|---|---|
| `NO_MATTER` | 0 | Any fill level accepted |
| `EMPTY` | 1 | Item must be empty |
| `FULL` | 2 | Item must be full |
| `COEFFICIENT` | 3 | Custom fill percentage |
| `STATIC` | 4 | Fixed quantity |

**Buy Modes** (how full is the item when bought):
| Mode | Value | Description |
|---|---|---|
| `EMPTY` | 0-1 | Item spawns empty |
| `FULL` | 2 | Item spawns full |
| `COEFFICIENT` | 3 | Custom fill percentage |
| `STATIC` | 4 | Fixed quantity |

> In CSV format, these are separate columns: `buyQtyMode`, `buyQtyValue`, `sellQtyMode`, `sellQtyValue`.

### How to add a new product (Legacy)

1. Create a new JSON file in `Products/` (you can copy an existing one)
2. Change the `className` to the DayZ class name of your item
3. Set `buyPrice` and `sellPrice` (use `-1` to disable buy or sell)
4. Set `maxStock` (`-1` for unlimited)
5. Clear the `productId` field to `""` — it will be auto-generated on next server start
6. Add the new product's UUID to a category's `productIds` array (see Section 7)
7. Restart the server

### How to add attachments to a product

1. Create product JSON files for each attachment (e.g., magazine, scope, stock)
2. Copy each attachment's `productId` (UUID)
3. Add those UUIDs to the parent product's `attachments` array
4. Restart the server — the customize page will show these as selectable attachments

### How to add variants to a product

Variants are alternative versions of the same item (e.g., AKM with different colors).

1. Create product JSON files for each variant
2. Copy each variant's `productId`
3. Add those UUIDs to the base product's `variants` array
4. Restart the server

---

## 7. Categories Configuration

A **category** groups products together in the trader menu (e.g., "Assault Rifles", "Vehicles", "Medical").

### Legacy Format — `Categories/<uuid>.json`

```json
{
    "isVisible": true,
    "icon": "set:dayz_gui image:icon_weapons",
    "categoryName": "Assault Rifles",
    "licensesRequired": [],
    "productIds": [
        "uuid-of-akm",
        "uuid-of-m4a1",
        "uuid-of-ak74"
    ],
    "categoryId": "auto-generated-uuid"
}
```

### Category Field Reference

| Field | Type | Default | Description |
|---|---|---|---|
| `categoryName` | string | — | Display name shown in the trader menu |
| `icon` | string | `""` | Icon identifier (DayZ image set format) |
| `isVisible` | bool | `true` | If `false`, category is hidden from the menu |
| `licensesRequired` | array | `[]` | License names required to access this category |
| `productIds` | array | `[]` | List of product UUIDs belonging to this category |
| `categoryId` | string | auto | Unique identifier. Auto-generated, do not edit |

### How to create a new category

1. Create a new JSON file in `Categories/` (copy an existing one)
2. Set `categoryName` to the display name (e.g., `"Sniper Rifles"`)
3. Set `icon` if desired (or leave empty)
4. Clear `categoryId` to `""` — auto-generated on restart
5. Add product UUIDs to `productIds`
6. Add the new category's UUID to a trader's `categoriesId` array in `TraderXGeneralSettings.json`
7. Restart the server

### How to restrict a category with a license

1. In `TraderXGeneralSettings.json`, add a license to the `licenses` array:
   ```json
   { "licenseName": "Weapons License", "description": "Required to buy weapons" }
   ```
2. In the category JSON, add the license name to `licensesRequired`:
   ```json
   "licensesRequired": ["Weapons License"]
   ```
3. Players must own the license to see/buy from this category

---

## 8. NPC Traders — How to Create and Configure

Traders are NPC characters that players interact with to buy and sell items. They are defined in the `traders` array inside `TraderXGeneralSettings.json`.

### Trader NPC Structure

```json
{
    "npcId": 1,
    "className": "SurvivorM_Mirek",
    "givenName": "Boris",
    "role": "Weapons Dealer",
    "position": [11600.0, 58.5, 14670.0],
    "orientation": [62, 0, 0],
    "categoriesId": [
        "uuid-of-assault-rifles-category",
        "uuid-of-sniper-rifles-category"
    ],
    "currenciesAccepted": [
        "EUR"
    ],
    "loadouts": [
        {
            "className": "GorkaHelmet",
            "quantity": -1,
            "slotName": "",
            "attachments": []
        },
        {
            "className": "GorkaEJacket_Autumn",
            "quantity": -1,
            "slotName": "",
            "attachments": []
        }
    ]
}
```

### Trader Field Reference

| Field | Type | Description |
|---|---|---|
| `npcId` | int | Auto-generated unique ID. Do not set manually. |
| `className` | string | DayZ survivor class name for the NPC model (e.g., `"SurvivorM_Mirek"`, `"SurvivorF_Linda"`) |
| `givenName` | string | Display name shown in the trader menu header |
| `role` | string | Role/title shown below the name (e.g., `"Weapons Dealer"`, `"Vehicle Trader"`) |
| `position` | vector | World position `[x, y, z]` where the NPC spawns |
| `orientation` | vector | Direction the NPC faces `[yaw, pitch, roll]` |
| `categoriesId` | array | List of category UUIDs this trader sells. **This links categories to the trader.** |
| `currenciesAccepted` | array | List of currency type names (from `TraderXCurrencySettings.json`) this trader accepts |
| `loadouts` | array | Clothing/gear the NPC wears (cosmetic only) |

### How to create a new trader

1. Open `TraderXGeneralSettings.json`
2. Add a new entry to the `traders` array (copy an existing trader as template)
3. Set `className` to a DayZ survivor model (see list below)
4. Set `givenName` and `role`
5. Set `position` to the world coordinates where you want the NPC (use an admin tool to find coordinates)
6. Set `orientation` — the yaw (first value) controls which direction the NPC faces
7. Add category UUIDs to `categoriesId` — these are the categories this trader will sell
8. Add currency names to `currenciesAccepted` — must match names from `TraderXCurrencySettings.json`
9. Optionally configure `loadouts` for NPC appearance
10. Restart the server

### Common NPC Class Names

| Class Name | Description |
|---|---|
| `SurvivorM_Mirek` | Male survivor (Mirek) |
| `SurvivorM_Denis` | Male survivor (Denis) |
| `SurvivorM_Boris` | Male survivor (Boris) |
| `SurvivorM_Cyril` | Male survivor (Cyril) |
| `SurvivorM_Elias` | Male survivor (Elias) |
| `SurvivorM_Peter` | Male survivor (Peter) |
| `SurvivorF_Linda` | Female survivor (Linda) |
| `SurvivorF_Maria` | Female survivor (Maria) |
| `SurvivorF_Frida` | Female survivor (Frida) |
| `SurvivorF_Gabi` | Female survivor (Gabi) |
| `SurvivorF_Naomi` | Female survivor (Naomi) |

### NPC Loadouts (Clothing)

Each loadout item dresses the NPC. The NPC's appearance is cosmetic only.

```json
{
    "className": "GorkaHelmet",
    "quantity": -1,
    "slotName": "",
    "attachments": [
        { "className": "GorkaHelmetVisor", "quantity": 1 }
    ]
}
```

| Field | Type | Description |
|---|---|---|
| `className` | string | DayZ class name of the clothing/gear item |
| `quantity` | int | `-1` = default. For items with quantity (e.g., ammo), set the amount |
| `slotName` | string | Force a specific attachment slot (leave `""` for auto) |
| `attachments` | array | Sub-attachments on this loadout item (e.g., visor on helmet) |

### How to make a trader sell vehicles

1. Create product entries for each vehicle (e.g., `OffroadHatchback`, `CivilianSedan`)
2. Create a "Vehicles" category and add those product UUIDs
3. Add the category UUID to the trader's `categoriesId`
4. Set up vehicle parking positions (see [Section 9](#9-vehicle-parking))
5. Restart the server

---

## 9. Vehicle Parking

When a player buys a vehicle, it needs a physical location to spawn. Each trader that sells vehicles needs **parking positions** configured.

### Directory: `TraderXConfig/TraderXVehicleParking/`

### Parking Positions — `parkings_<npcId>.json`

One file per trader NPC. The `<npcId>` must match the trader's `npcId` from `TraderXGeneralSettings.json`.

```json
{
    "traderId": "1",
    "positions": [
        {
            "position": [11615.0, 59.0, 14690.0],
            "rotation": [50, 0, 0],
            "size": [3, 2, 6]
        },
        {
            "position": [11620.0, 59.0, 14695.0],
            "rotation": [50, 0, 0],
            "size": [3, 2, 6]
        }
    ]
}
```

| Field | Type | Description |
|---|---|---|
| `traderId` | string | Must match the NPC's `npcId` |
| `positions` | array | List of parking slots |
| `positions[].position` | vector | World position `[x, y, z]` where the vehicle spawns |
| `positions[].rotation` | vector | Vehicle rotation `[yaw, pitch, roll]` |
| `positions[].size` | vector | Bounding box `[width, height, length]` — used to check if the spot is clear |

### Parking Config — `parking_config.json`

Controls which world objects are ignored when checking if a parking spot is clear.

```json
{
    "whitelistedObjects": [
        "Land_Grass",
        "Land_Stone",
        "Land_Dirt",
        "ClutterCutter",
        "Particle",
        "Terrain"
    ]
}
```

Objects whose class name contains any of these strings will not block vehicle spawning.

### How to set up vehicle parking

1. Go to the trader location in-game with an admin tool
2. Find flat areas near the trader where vehicles should spawn
3. Note the world coordinates and desired rotation for each spot
4. Create `parkings_<npcId>.json` in the `TraderXVehicleParking/` folder
5. Add a position entry for each parking slot
6. Set `size` to roughly match the vehicle dimensions (default `[3, 2, 6]` works for most cars)
7. Restart the server

> **Tip:** Add more parking spots than you think you need. If all spots are occupied, vehicle purchases will fail.

---

## 10. Server Presets

Server presets are **admin-configured weapon loadouts** that appear in the customize page. Players can also create their own presets in-game.

### Directory: `TraderXPresets/`

Preset files are stored per product: `presets_<productId>.json`

```json
{
    "productId": "uuid-of-akm",
    "defaultPresetId": "preset_uuid_tactical_001",
    "presets": [
        {
            "presetName": "Tactical Loadout",
            "presetId": "preset_uuid_tactical_001",
            "productId": "uuid-of-akm",
            "attachments": [
                "uuid-of-akm-mag-30",
                "uuid-of-pso1-scope",
                "uuid-of-akm-buttstock"
            ]
        },
        {
            "presetName": "CQB Setup",
            "presetId": "preset_uuid_cqb_001",
            "productId": "uuid-of-akm",
            "attachments": [
                "uuid-of-akm-mag-30",
                "uuid-of-akm-rail"
            ]
        }
    ]
}
```

### Preset Field Reference

| Field | Type | Description |
|---|---|---|
| `productId` | string | The product UUID this preset belongs to |
| `defaultPresetId` | string | The `presetId` of the default preset (auto-selected on open) |
| `presets` | array | List of preset configurations |
| `presets[].presetName` | string | Display name (e.g., `"Tactical Loadout"`) |
| `presets[].presetId` | string | Unique preset identifier (auto-generated) |
| `presets[].productId` | string | The base product UUID |
| `presets[].attachments` | array | List of attachment product UUIDs included in this preset |

### How to create a server preset

Server presets can also be defined via CSV (see [Section 14](#14-csv-format-reference)). For manual JSON:

1. Find the `productId` of the weapon you want to create a preset for
2. Find the `productId` of each attachment you want in the preset
3. Create or edit `presets_<productId>.json` in `TraderXPresets/`
4. Add a preset entry with a descriptive `presetName` and the attachment UUIDs
5. Set `presetId` to `""` — it will be auto-generated
6. Optionally set `defaultPresetId` to auto-select this preset
7. Restart the server

> **Note:** Player-created presets are stored in the same files. Server presets are distinguished by being pre-configured before the player interacts with the system.

---

## 11. Licenses

Licenses restrict access to specific categories. Players must own the required license to buy from a licensed category.

### How licenses work

1. **Define licenses** in `TraderXGeneralSettings.json` → `licenses` array
2. **Assign to categories** via the category's `licensesRequired` array
3. **Grant to players** programmatically or via admin tools

### License Structure

```json
{
    "licenseId": "auto-generated-uuid",
    "licenseName": "Car License",
    "description": "Allows you to drive a car."
}
```

### Player License Data — `TraderXLicenses/playerLicense_<steamId>.json`

Auto-managed. Each player's owned licenses are stored here:

```json
{
    "steamId": "76561198012345678",
    "playerName": "PlayerName",
    "licenses": [
        {
            "licenseId": "uuid-matching-car-license",
            "licenseName": "Car License",
            "description": "Allows you to drive a car."
        }
    ]
}
```

> **Note:** License files are auto-managed by the mod. You generally don't edit these manually unless troubleshooting.

---

## 12. Dynamic Translations

### File: `TraderXConfig/TraderXDynamicTranslationSettings.json`

All user-facing messages can be customized here. Useful for translating to other languages or changing wording.

```json
{
    "version": "1.0.1",
    "successfullyBought": "You successfully bought %1",
    "successfullySold": "You successfully sold %1",
    "failedToBuy": "Failed to buy %1: %2",
    "failedToSell": "Failed to sell %1: %2",
    "insufficientFunds": "Insufficient funds",
    "insufficientStock": "Insufficient stock for quantity requested",
    "purchaseSuccessful": "Purchase successful",
    "saleSuccessful": "Sale successful",
    "invalidTransaction": "Invalid transaction",
    "productNotFound": "Product not found",
    "traderNotFound": "Trader not found",
    "traderStockFull": "Trader stock is full",
    "itemNotFoundInInventory": "Item not found in inventory",
    "invalidPrice": "Invalid price - transactions with negative prices are not allowed",
    "vehicleMultipleQuantity": "Vehicles can only be purchased one at a time",
    "vehicleSellMultipleQuantity": "Vehicles can only be sold one at a time",
    "bulkTransactionResult": "Bulk Transaction Result",
    "bulkTransactionSummary": "%1 out of %2 transactions were successful",
    "completed": "Completed",
    "failedWithReason": "Failed: %1",
    "invalidPlayer": "Invalid player",
    "invalidPresetDetected": "Security: Invalid preset detected",
    "couldNotCreateWeapon": "Could not create weapon %1",
    "couldNotCreateItem": "Could not create item",
    "attachmentProductNotFound": "Attachment product not found: %1",
    "attachmentOutOfStock": "Attachment out of stock: %1",
    "presetValidationSuccessful": "Preset validation successful",
    "failedToApplyPreset": "Failed to apply preset: Attachment not found: %1",
    "couldNotCreateAttachment": "Failed to apply preset: Could not create attachment: %1",
    "presetAppliedSuccessfully": "Preset applied successfully",
    "purchaseFailedPrefix": "Purchase failed: ",
    "saleFailedPrefix": "Sale failed: "
}
```

### Placeholders

Messages use `%1`, `%2`, etc. as placeholders:
- `%1` — typically the item name or first value
- `%2` — typically the reason or second value

**Example — Translating to French:**
```json
{
    "successfullyBought": "Vous avez acheté %1 avec succès",
    "insufficientFunds": "Fonds insuffisants",
    "purchaseSuccessful": "Achat réussi"
}
```

> **Tip:** You only need to change the values you want to customize. Missing fields use built-in defaults.

---

## 13. Debug Settings

### File: `TraderXConfig/TraderXDebugSettings.json`

Controls debug mode and automated test execution. **Only for development/troubleshooting.**

```json
{
    "version": "1.0.1",
    "enableDebugMode": false,
    "runTestsOnStartup": false,
    "runCurrencyServiceTests": false,
    "runTransactionServiceTests": true,
    "runVehicleTransactionTests": false,
    "runPricingServiceTests": true,
    "runJSONTestCases": true,
    "authorizedAdmins": [
        "76561198047475641"
    ]
}
```

| Field | Type | Default | Description |
|---|---|---|---|
| `enableDebugMode` | bool | `false` | Enable verbose debug logging |
| `runTestsOnStartup` | bool | `false` | Run test suites when server starts |
| `runCurrencyServiceTests` | bool | `false` | Test currency calculations |
| `runTransactionServiceTests` | bool | `true` | Test buy/sell transactions |
| `runVehicleTransactionTests` | bool | `false` | Test vehicle transactions |
| `runPricingServiceTests` | bool | `true` | Test pricing calculations |
| `runJSONTestCases` | bool | `true` | Run test cases from `DebugTestCases/` folder |
| `authorizedAdmins` | array | `[]` | Steam64 IDs of players allowed to trigger debug features |

> **Warning:** Keep `enableDebugMode` set to `false` in production. It generates very verbose logs.

---

## 14. CSV Format Reference

CSV files use **tab-delimited** format. The first row is always the header. You can edit them in Excel, Google Sheets, LibreOffice Calc, or any text editor.

### `products.csv` — Column Reference

```
productKey	className	buyPrice	sellPrice	maxStock	buyQtyMode	buyQtyValue	sellQtyMode	sellQtyValue	destockCoefficient	stockBehaviorAtRestart	attachments	variants	notes
```

| Column | Type | Description |
|---|---|---|
| `productKey` | string | Unique identifier for this product (e.g., `"akm"`, `"mag_akm_30"`) |
| `className` | string | DayZ class name (e.g., `"AKM"`) |
| `buyPrice` | int | Price to buy. `-1` = cannot buy |
| `sellPrice` | int | Price to sell. `-1` = cannot sell |
| `maxStock` | int | Max stock. `-1` = unlimited |
| `buyQtyMode` | string | `"EMPTY"`, `"FULL"`, `"COEFFICIENT"`, `"STATIC"` |
| `buyQtyValue` | float | Value for COEFFICIENT/STATIC buy mode (0.0-1.0 for coefficient) |
| `sellQtyMode` | string | `"NO_MATTER"`, `"EMPTY"`, `"FULL"`, `"COEFFICIENT"`, `"STATIC"` |
| `sellQtyValue` | float | Value for COEFFICIENT/STATIC sell mode |
| `destockCoefficient` | float | Stock decrease rate per cycle (0.0 = none, 1.0 = full destock) |
| `stockBehaviorAtRestart` | int | `0` = keep stock, `1` = reset to max, `2` = random |
| `attachments` | string | Semicolon-separated productKeys (e.g., `"mag_akm_30;pso1_scope"`) |
| `variants` | string | Semicolon-separated productKeys |
| `notes` | string | Admin notes (ignored by the game) |

**Example rows:**
```
akm	AKM	5000	2500	10	FULL	0	FULL	0	0	0	mag_akm_30;akm_buttstock	akm_green;akm_black	Main assault rifle
mag_akm_30	Mag_AKM_30Rnd	200	100	-1	FULL	0	NO_MATTER	0	0	0			AKM 30-round magazine
```

### `categories.csv` — Column Reference

```
categoryKey	categoryName	icon	isVisible	licensesRequired	productKeys	notes
```

| Column | Type | Description |
|---|---|---|
| `categoryKey` | string | Unique identifier (e.g., `"weapons_assault"`) |
| `categoryName` | string | Display name (e.g., `"Assault Rifles"`) |
| `icon` | string | Icon identifier (DayZ image set format, or empty) |
| `isVisible` | int | `1` = visible, `0` = hidden |
| `licensesRequired` | string | Semicolon-separated license names (e.g., `"Weapons License"`) |
| `productKeys` | string | Semicolon-separated productKeys (e.g., `"akm;m4a1;ak74"`) |
| `notes` | string | Admin notes (ignored by the game) |

**Example rows:**
```
weapons_assault	Assault Rifles	set:dayz_gui image:icon_weapons	1		akm;m4a1;ak74	All assault rifles
vehicles_cars	Cars	set:dayz_gui image:icon_car	1	Car License	offroad;sedan;hatchback	Requires car license
```

### CSV Packs (Multiple Files)

You can split products/categories across multiple CSV files. Files are merged alphabetically by filename. Use a numeric prefix to control load order:

```
Source/
├── 10_weapons_products.csv      # Loaded first
├── 10_weapons_categories.csv
├── 20_vehicles_products.csv     # Loaded second
├── 20_vehicles_categories.csv
├── 30_medical_products.csv      # Loaded third
└── 30_medical_categories.csv
```

The system detects whether a CSV file contains products or categories based on the header row.

### Presets in CSV

Presets can also be defined in CSV source files:

```
presetKey	productKey	presetName	attachments	notes
preset_akm_tactical	akm	Tactical Loadout	mag_akm_30;pso1_scope;akm_buttstock	Full tactical setup
preset_akm_cqb	akm	CQB Setup	mag_akm_30;akm_rail	Close quarters
```

---

## 15. Migration Guide

### Legacy → CSV

1. Set `SourceConfig.json`:
   ```json
   { "preferredFormat": "CSV", "autoMigrate": true, "allowCompilation": true }
   ```
2. Start the server — legacy files are converted to `Source/products_migrated.csv` and `Source/categories_migrated.csv`
3. Rename the migrated files to `products.csv` and `categories.csv`
4. Set `autoMigrate` back to `false`
5. From now on, edit the CSV files and restart to apply changes

### Legacy → Compiled

1. Set `SourceConfig.json`:
   ```json
   { "preferredFormat": "COMPILED", "autoMigrate": true, "allowCompilation": true }
   ```
2. Start the server — legacy → CSV → compiled automatically
3. Set `autoMigrate` back to `false`
4. Edit CSV files in `Source/` or compiled JSON in `Compiled/` directly

### CSV → Compiled (automatic)

When `preferredFormat` is `"CSV"` and `allowCompilation` is `true`, CSV files are **automatically compiled** to `Compiled/` on every server start. The previous compiled files are backed up to `Backup/`.

### Important Notes

- **Original files are NEVER deleted** during migration
- A backup is created before each compilation
- Check `TraderXLogs/Config/ConfigReport.log` after migration for any validation warnings
- If compilation fails, the system falls back to the previous backup

---

## 16. FAQ — Common Questions

### General

**Q: Where are the config files located?**
A: Under your server's `$profile/TraderX/TraderXConfig/` directory. If you use `-profiles=myProfile`, look in `myProfile/TraderX/TraderXConfig/`.

**Q: Do I need to restart the server after editing configs?**
A: Yes. All configuration files are loaded at server startup. Changes require a restart.

**Q: The server created default files but they're empty / have example data. What do I do?**
A: The default files contain example traders and objects on Chernarus. Edit them to match your server's map and desired setup.

### Traders

**Q: How do I create a new trader NPC?**
A: Add a new entry to the `traders` array in `TraderXGeneralSettings.json`. See [Section 8](#8-npc-traders) for the full structure and step-by-step guide.

**Q: How do I move a trader to a different location?**
A: Change the `position` and `orientation` fields in the trader's entry in `TraderXGeneralSettings.json`. Use an admin tool in-game to find the exact coordinates.

**Q: How do I change what a trader sells?**
A: Edit the `categoriesId` array in the trader's entry. Add or remove category UUIDs to control which categories appear in that trader's menu.

**Q: Can multiple traders share the same categories?**
A: Yes. Add the same category UUID to multiple traders' `categoriesId` arrays.

**Q: How do I dress/customize the trader NPC appearance?**
A: Edit the `loadouts` array in the trader entry. Each loadout item is a clothing/gear class name. See [Section 8 — NPC Loadouts](#npc-loadouts-clothing).

### Products

**Q: How do I add a new item to a trader?**
A: Create a product (Legacy: new JSON in `Products/`, CSV: new row in `products.csv`), then add its ID to a category, then add that category to a trader. See [Section 6](#6-products-configuration).

**Q: How do I make an item buy-only (cannot be sold)?**
A: Set `sellPrice` to `-1`.

**Q: How do I make an item sell-only (cannot be bought)?**
A: Set `buyPrice` to `-1`.

**Q: How do I set unlimited stock?**
A: Set `maxStock` to `-1`.

**Q: How do I add attachments to a weapon?**
A: Create product entries for each attachment, then add their UUIDs/productKeys to the weapon's `attachments` array. See [Section 6 — How to add attachments](#how-to-add-attachments-to-a-product).

**Q: What's the difference between attachments and variants?**
A: **Attachments** are items that go ON the product (scopes, magazines, stocks). **Variants** are alternative versions of the same product (different colors, models). Both appear in the customize page.

**Q: How does stock work?**
A: Each product has a `maxStock`. When a player buys, stock decreases. When a player sells, stock increases. Set `maxStock` to `-1` for unlimited. Stock data is stored in `TraderXDatabase/` and persists across restarts (unless `stockBehaviorAtRestart` is set to reset).

**Q: What does stockBehaviorAtRestart do?**
A: Controls what happens to stock on server restart:
- `0` = Keep current stock levels
- `1` = Reset all stock to `maxStock`
- `2` = Randomize stock between 0 and `maxStock`

### Categories

**Q: How do I create a new category?**
A: See [Section 7](#7-categories-configuration). Create a category file/row, add products to it, then assign it to a trader.

**Q: How do I hide a category from the menu?**
A: Set `isVisible` to `false` (Legacy JSON) or `0` (CSV).

**Q: How do I require a license to access a category?**
A: Add the license name to the category's `licensesRequired` array. See [Section 7 — How to restrict a category with a license](#how-to-restrict-a-category-with-a-license).

### Currency

**Q: How do I add a new currency?**
A: Add a new currency type to `TraderXCurrencySettings.json` with denominations, then add the currency name to a trader's `currenciesAccepted` array. See [Section 5](#5-currency-configuration).

**Q: Can a trader accept multiple currencies?**
A: Yes. Add multiple currency names to the trader's `currenciesAccepted` array (e.g., `["EUR", "Gold"]`).

**Q: Can I use items from other mods as currency?**
A: Yes. Set the `className` in the currency denomination to any valid DayZ item class name, including modded items.

### Vehicles

**Q: How do I set up vehicle trading?**
A: Create vehicle products, add them to a category, assign the category to a trader, then configure parking positions. See [Section 8 — How to make a trader sell vehicles](#how-to-make-a-trader-sell-vehicles) and [Section 9](#9-vehicle-parking).

**Q: Vehicle purchases fail with "no parking available". What do I do?**
A: You need to configure parking positions for that trader. Create a `parkings_<npcId>.json` file in `TraderXVehicleParking/`. Also check that existing spots aren't blocked by objects or other vehicles.

### CSV / Format

**Q: Which format should I use?**
A: **Legacy** for small servers or if you prefer editing individual files. **CSV** for medium/large servers or if you want spreadsheet editing. **Compiled** for maximum performance.

**Q: Can I switch formats after initial setup?**
A: Yes. Change `preferredFormat` in `SourceConfig.json` and set `autoMigrate: true` for the first run. Original files are never deleted.

**Q: My CSV changes aren't taking effect. Why?**
A: Make sure `preferredFormat` is set to `"CSV"` (not `"LEGACY"`) in `SourceConfig.json`. CSV files are compiled on server start — check `ConfigReport.log` for errors.

**Q: Can I use Excel to edit CSV files?**
A: Yes, but make sure to save as **Tab-delimited text** (not comma-separated). The delimiter must be TAB, not comma.

### Troubleshooting

**Q: Where are the logs?**
A: `TraderX/TraderXLogs/Logs/TraderX_<date>.log` for general logs. `TraderX/TraderXLogs/Config/ConfigReport.log` for configuration validation.

**Q: The trader menu is empty / no products showing.**
A: Check that:
1. Products exist in the correct format folder (Legacy/CSV/Compiled)
2. Products are assigned to a category
3. The category is assigned to the trader's `categoriesId`
4. `SourceConfig.json` has the correct `preferredFormat`

**Q: Items have wrong prices or stock.**
A: Check the product configuration. In Legacy, check the individual JSON file. In CSV, check the row in `products.csv`. Make sure you're editing the correct format (matching `preferredFormat`).

**Q: Server crashes on startup related to TraderX.**
A: Check the server RPT log and `TraderX_<date>.log`. Common causes:
- Malformed JSON (missing comma, bracket, etc.)
- Invalid class names that don't exist in the game
- Circular references in attachments/variants
