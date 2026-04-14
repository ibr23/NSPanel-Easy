# Canvas Page

> [!WARNING]
> This page is intended for **advanced users only**. Using it requires knowledge of Home Assistant
> automations, YAML, Jinja2 templating, and the Nextion drawing instruction set. If you are new
> to any of these, a good starting point is to experiment with the built-in pages first. A capable
> AI assistant can also help you adapt the examples below to your specific use case.

The **canvas** page is a blank display page with a standard header (icon, title, and close button).
Everything below the header is empty and fully under your control — you draw content on it by
sending commands to the Nextion display from a Home Assistant automation.

This page is for users who want to display custom information that does not fit any of the
built-in pages.

## How it works

The pattern has two parts:

1. **A Home Assistant script** that navigates to the canvas page. You assign this script to a
   button in the project so that pressing the button opens the page.
2. **A Home Assistant automation** triggered when the panel reports that the canvas page is active.
   The automation draws whatever content you want by sending Nextion commands through the
   `esphome.<panel_name>_command` action.

## Step 1 — Create a Home Assistant script to open the page

Create a script in Home Assistant that sends the `page canvas` command to the panel:

```yaml
script:
  open_canvas:
    alias: Open canvas page
    sequence:
      - action: esphome.<panel_name>_command
        data:
          cmd: "page canvas"
```

You can now assign this script to any button in the project via the Blueprint's button
configuration. Pressing that button will open the canvas page.

### Setting the return page (optional)

By default, pressing the close button on the canvas page returns to the home page.
You can change the return destination by setting `back_page_id` to the numeric ID of the target
page before navigating:

```yaml
script:
  open_canvas_from_buttonpage:
    alias: Open canvas page (return to calling page)
    sequence:
      - action: esphome.<panel_name>_command
        data:
          cmd: "back_page_id=dp"  # dp = current page ID; use a numeric ID instead to return to a specific page
      - action: esphome.<panel_name>_command
        data:
          cmd: "page canvas"
```

## Step 2 — Draw content from a Home Assistant automation

Create an automation triggered by the `sensor.<panel_name>_current_page` sensor changing to
`canvas`. Inside the automation, call `esphome.<panel_name>_command` once per Nextion instruction,
and use `esphome.<panel_name>_component_text` to set the header icon and title.

The Nextion display uses a simple drawing instruction set. The key commands are:

| Command | Purpose |
| ------- | ------- |
| `fill x,y,w,h,color` | Fill a rectangle with a solid color |
| `xstr x,y,w,h,font,fg,bg,align,xcen,ycen,"text"` | Draw text inside a bounding box |
| `cirs x,y,r,color` | Draw a filled circle (used for rounded-rectangle corners) |
| `line x1,y1,x2,y2,color` | Draw a straight line |

Colors are in **RGB565** format (16-bit integer). A full reference for the drawing commands is
available in the [Nextion Instruction Set — GUI Designing Commands](https://nextion.tech/instruction-set/#s4).

<!-- markdownlint-disable MD028 -->
> [!IMPORTANT]
> Each command string sent to the display must be **under 255 bytes** after all variables are
> substituted. Keep text strings short and avoid complex expressions inside a single `cmd` value.

> [!NOTE]
> **Add small delays between commands.** Home Assistant dispatches actions faster than the ESP32
> UART queue can drain them. Without delays, some commands arrive at the display out of order or
> are rendered after subsequent commands have already painted over their area. A `delay` of
> **10 ms between individual commands** and **50 ms between logical groups** (such as rows in a
> table) is sufficient in practice.
<!-- markdownlint-enable MD028 -->

### Header components

The canvas page header exposes two components you can set via `component_text`:

- `icon_state` — MDI icon, uses the icon font. Set to the icon's Unicode codepoint.
- `page_label` — title text, displayed next to the icon.

```yaml
- action: esphome.<panel_name>_component_text
  data:
    page: canvas
    id: icon_state
    txt: "\uE159"  # mdi:bus

- action: esphome.<panel_name>_component_text
  data:
    page: canvas
    id: page_label
    txt: "My title"
```

### `xstr` text background

Nextion's `xstr` command always paints a solid background behind the text — there is no
transparency. Set the `bg` parameter to match the background color of whatever surface the text
sits on. If you draw a card rectangle first, use the card color as `bg` for all text inside it.

## Example: bus departure board

This automation draws a departure board with a card background, a populated header, and up to
5 rows of departures. All geometry and appearance values are in top-level variables for easy
adjustment.

> [!TIP]
> The departure data below is hard-coded for demonstration purposes. In a real application,
> replace the `departures` variable with data from a Home Assistant sensor — for example, a
> RESTful sensor or a custom integration that fetches live departure times from a public
> transit API. The rest of the automation does not need to change.

### Script to open the page

```yaml
script:
  canvas_bus_departures:
    alias: "Open bus departures"
    sequence:
      - action: esphome.<panel_name>_command
        data:
          cmd: "page canvas"
```

### Automation to draw the board

```yaml
automation:
  - alias: Canvas - Bus departures
    triggers:
      - trigger: state
        entity_id: sensor.<panel_name>_current_page
        to: canvas
    variables:
      # Appearance
      font: 3
      card_bg: 6339        # RGB565_GRAY_DARKEST — adjust to taste
      card_margin: 8
      badge_color: 1694    # transit blue
      # Card geometry
      card_x: 4
      card_y: 52           # just below the 48px header
      card_w: 442          # fits within 450px visible width
      card_h: 248          # fits within 300px visible height limit
      # Table geometry (derived from card)
      row_h: 44
      col_badge_x: "{{ card_x + card_margin }}"
      col_dest_x: "{{ card_x + card_margin + 54 }}"
      col_time_x: "{{ card_x + card_w - card_margin - 72 }}"
      col_time_w: 72
      table_y: "{{ card_y + card_margin }}"
      max_rows: "{{ ((card_h - card_margin * 2) / row_h) | int }}"
      # Departure data — replace with a sensor later
      departures:
        - route: "2"
          destination: "Jelitkowo"
          minutes: "now"
        - route: "8"
          destination: "Jelitkowo"
          minutes: "4 min"
        - route: "2"
          destination: "Lawendowe Wzgorze"
          minutes: "7 min"
        - route: "5"
          destination: "Nowy Port"
          minutes: "10 min"
        - route: "5"
          destination: "Oliwa"
          minutes: "12 min"
    actions:
      # Header icon (mdi:bus) and stop name
      - action: esphome.<panel_name>_component_text
        data:
          page: canvas
          id: icon_state
          txt: "\uE159"

      - action: esphome.<panel_name>_component_text
        data:
          page: canvas
          id: page_label
          txt: "Wyspianski"

      # Card background — drawn once before the loop
      - action: esphome.<panel_name>_command
        data:
          cmd: "fill {{ card_x }},{{ card_y }},{{ card_w }},{{ card_h }},{{ card_bg }}"

      # Allow the card fill to complete before drawing text on top
      - delay:
          milliseconds: 20

      # Rows
      - repeat:
          count: "{{ [departures | count, max_rows] | min }}"
          sequence:
            - variables:
                dep: "{{ departures[repeat.index - 1] }}"
                ry: "{{ table_y + (repeat.index - 1) * row_h }}"
                badge_y: "{{ ry + (row_h - 36) // 2 }}"

            # Route badge background
            - action: esphome.<panel_name>_command
              data:
                cmd: "fill {{ col_badge_x }},{{ badge_y }},44,36,{{ badge_color }}"

            # Small delay to ensure fill completes before text is drawn on top
            - delay:
                milliseconds: 10

            # Route number centred in badge
            - action: esphome.<panel_name>_command
              data:
                cmd: "xstr {{ col_badge_x }},{{ badge_y }},44,36,{{ font }},65535,{{ badge_color }},1,1,1,\"{{ dep.route }}\""

            - delay:
                milliseconds: 10

            # Destination label
            - action: esphome.<panel_name>_command
              data:
                cmd: "xstr {{ col_dest_x }},{{ ry }},{{ col_time_x - col_dest_x - 4 }},{{ row_h }},{{ font }},65535,{{ card_bg }},0,1,1,\"{{ dep.destination }}\""

            - delay:
                milliseconds: 10

            # Arrival time, right-aligned
            - action: esphome.<panel_name>_command
              data:
                cmd: "xstr {{ col_time_x }},{{ ry }},{{ col_time_w }},{{ row_h }},{{ font }},65535,{{ card_bg }},2,1,1,\"{{ dep.minutes }}\""

            # Allow the full row to render before starting the next one
            - delay:
                milliseconds: 50
```

## Nextion coordinate reference

### Landscape (EU and US Landscape)

Display resolution: 480×320 px. The right 30 px (x=450 to x=479) are not visible on EU model,
so the effective drawing area is 450×320 px.

```txt
(0,0) ────────────────────── (449,0)  ← 450px visible width
  │  header (icon/title/close) │   ← 48 px tall
  │────────────────────────────│
  │                            │
  │     your content area      │   ← y=50 to y=319
  │                            │
(0,319) ──────────────────(449,319)
```

### Portrait (US Portrait)

Display resolution: 320×480 px. The full width is visible.

```txt
(0,0) ──────────── (319,0)   ← 320px visible width
  │  header         │   ← 48 px tall
  │─────────────────│
  │                 │
  │  content area   │   ← y=50 to y=479
  │                 │
(0,479) ────────(319,479)
```

The `xstr` alignment parameter (7th positional): `0` = left, `1` = centre, `2` = right.

## RGB565 color reference

Use the [Nextion HMI Color Converter](https://nodtem66.github.io/nextion-hmi-color-convert/index.html)
to convert any RGB color to the RGB565 integer value used in drawing commands.

Some common values for quick reference:

| Color | Value |
| ----- | ----- |
| Black | `0` |
| White | `65535` |
| Light gray | `52857` |
| Dark gray | `16904` |
| Darkest gray | `6339` |
| Blue (transit) | `1694` |
| Green | `19818` |
| Red | `63488` |
| Orange | `64704` |
| Indigo | `10597` |
