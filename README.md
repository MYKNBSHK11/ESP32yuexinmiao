# ESP32-S3 + ST7789 SPI LCD 动画播放器

基于 ESP-IDF v6.0.1 和 LVGL v9.3，在 240×240 ST7789 屏幕上播放帧动画或 GIF 动图。

> 注：已针对 **8针 SPI LCD 屏幕**（无 MISO）和 **ESP32-S3** 优化。

---

## 硬件接线

| LCD 排针 | ESP32-S3 GPIO | 功能 |
|----------|---------------|------|
| 1 GND    | GND           | 电源地 |
| 2 VCC    | 3.3V          | 电源正 |
| 3 SCL    | GPIO 12       | SPI 时钟 (FSPI CLK) |
| 4 SDA    | GPIO 11       | SPI 数据 (FSPI MOSI) |
| 5 RES    | GPIO 4        | 复位 |
| 6 DC     | GPIO 5        | 数据/命令 |
| 7 CS     | GPIO 10       | 片选 (FSPI CS) |
| 8 BLK    | GPIO 6        | 背光（高电平亮） |

> 修改引脚：编辑 `main/spi_lcd_touch_example_main.c` 中的 `EXAMPLE_PIN_NUM_*` 宏。

---

## 文件结构

```
spi_lcd_touch/
├── CMakeLists.txt                     # ESP-IDF 工程入口
├── sdkconfig.defaults                 # 默认 Kconfig（LVGL + GIF 解码器）
├── README.md                          # 本文件
└── main/
    ├── CMakeLists.txt                 # 编译源文件列表
    ├── Kconfig.projbuild              # menuconfig 菜单（触摸开关）
    ├── idf_component.yml              # 依赖：lvgl 9.3.0、触摸驱动
    ├── spi_lcd_touch_example_main.c   # 主程序：SPI/LCD 初始化、LVGL 启动
    ├── lvgl_demo_ui.c                 # UI 层：播放动画/GIF
    └── animation_data.h               # 22 帧 128×64 黑白动画数据（可选）
```

### 各文件说明

| 文件 | 角色 | 修改频率 |
|------|------|:---:|
| `spi_lcd_touch_example_main.c` | 引脚配置、SPI 总线初始化、ST7789 驱动、LVGL 显示系统 | 🟡 偶尔 |
| `lvgl_demo_ui.c` | 自定义 UI / 动画播放逻辑 | 🟢 经常 |
| `animation_data.h` | 帧动画原始数据（OLED 格式） | 🔴 不改 |
| `sdkconfig.defaults` | LVGL 功能开关 | 🟡 偶尔 |
| `idf_component.yml` | 依赖库版本 | 🟡 偶尔 |

---

## 构建 & 烧录

```bash
# 首次使用需先设置 ESP-IDF 环境
idf.py set-target esp32s3

# 构建
idf.py build

# 烧录 + 串口监视
idf.py flash monitor

# 退出监视器：Ctrl+]
```

---

## 播放 GIF 动图

GIF 解码器已启用（`CONFIG_LV_USE_GIF=y`），使用流程：

### 1. 转换 GIF → C 数组

打开 [LVGL Image Converter](https://lvgl.io/tools/imageconverter)

- 上传 `.gif` 文件
- Color format：`CF_TRUE_COLOR`
- Output format：**`Raw G-I-F file`**
- 下载生成的 `.c` 文件，放到 `main/` 目录

### 2. 在代码中使用

编辑 `main/lvgl_demo_ui.c`，把 placeholder 替换为：

```c
extern const lv_image_dsc_t img_xxx;   // 用你 .c 文件里的变量名

void example_lvgl_demo_ui(lv_display_t *disp)
{
    lv_obj_t *scr = lv_display_get_screen_active(disp);
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    lv_obj_t *gif = lv_gif_create(scr);
    lv_gif_set_src(gif, &img_xxx);
    lv_obj_center(gif);
}
```

### 3. 如果新增了 `.c` 文件

在 `main/CMakeLists.txt` 中添加：

```cmake
idf_component_register(SRCS "spi_lcd_touch_example_main.c" "lvgl_demo_ui.c" "你的gif文件.c"
                       INCLUDE_DIRS ".")
```

---

## 配置选项

```bash
idf.py menuconfig
```

进入 `Example Configuration`：

| 选项 | 说明 |
|------|------|
| Enable LCD touch | 启用触摸（默认关闭） |
| LCD touch controller model | STMPE610 / XPT2046 |

---

## 关键参数

| 参数 | 值 | 位置 |
|------|-----|------|
| 分辨率 | 240×240 | `EXAMPLE_LCD_H_RES/V_RES` |
| SPI 时钟 | 10 MHz | `EXAMPLE_LCD_PIXEL_CLOCK_HZ` |
| SPI 模式 | Mode 0 | `io_config.spi_mode` |
| 驱动芯片 | ST7789 | `esp_lcd_new_panel_st7789()` |
| 颜色深度 | RGB565 (16bit) | `LV_COLOR_FORMAT_RGB565` |
| 背光 | 高电平亮 | `EXAMPLE_LCD_BK_LIGHT_ON_LEVEL = 1` |
| LVGL 版本 | 9.3.0 | `idf_component.yml` |
| ESP-IDF | v6.0.1 | — |
| 目标芯片 | ESP32-S3 | — |

---

## 依赖库

| 库 | 版本 |
|----|------|
| [lvgl](https://components.espressif.com/components/lvgl/lvgl) | 9.3.0 |
| esp_lcd_touch_stmpe610 | ^1.0.0 |
| esp_lcd_touch_xpt2046 | 1.0.6 |
