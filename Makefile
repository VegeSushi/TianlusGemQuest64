BUILD_DIR=build
include $(N64_INST)/include/n64.mk

src = main.c
assets_png = $(wildcard assets/*.png)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite)))

MKSPRITE_FLAGS ?=

all: tgq64.z64

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f RGBA16 -o "$(dir $@)" "$<"

$(BUILD_DIR)/tgq64.dfs: $(assets_conv)
$(BUILD_DIR)/tgq64.elf: $(src:%.c=$(BUILD_DIR)/%.o)

tgq64.z64: N64_ROM_TITLE="tianlusgemquest64"
tgq64.z64: $(BUILD_DIR)/tgq64.dfs

clean:
	rm -rf $(BUILD_DIR) filesystem/ tgq64.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean