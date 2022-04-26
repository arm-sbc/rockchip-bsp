/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _ROCKCHIP_PANEL_H_
#define _ROCKCHIP_PANEL_H_

struct display_state;
struct rockchip_panel;

struct rockchip_panel_funcs {
	void (*init)(struct rockchip_panel *panel);
	void (*prepare)(struct rockchip_panel *panel);
	void (*unprepare)(struct rockchip_panel *panel);
	void (*enable)(struct rockchip_panel *panel);
	void (*disable)(struct rockchip_panel *panel);
};

struct rockchip_panel {
	struct udevice *dev;
	u32 bus_format;
	unsigned int bpc;
	const struct rockchip_panel_funcs *funcs;
	const void *data;

	struct display_state *state;
};

static inline void rockchip_panel_init(struct rockchip_panel *panel)
{
	if (!panel)
		return;

	if (panel->funcs && panel->funcs->init)
		panel->funcs->init(panel);
}

static inline void rockchip_panel_prepare(struct rockchip_panel *panel)
{
	if (!panel)
		return;

	if (panel->funcs && panel->funcs->prepare)
		panel->funcs->prepare(panel);
}

static inline void rockchip_panel_enable(struct rockchip_panel *panel)
{
	if (!panel)
		return;

	if (panel->funcs && panel->funcs->enable)
		panel->funcs->enable(panel);
}

static inline void rockchip_panel_unprepare(struct rockchip_panel *panel)
{
	if (!panel)
		return;

	if (panel->funcs && panel->funcs->unprepare)
		panel->funcs->unprepare(panel);
}

static inline void rockchip_panel_disable(struct rockchip_panel *panel)
{
	if (!panel)
		return;

	if (panel->funcs && panel->funcs->disable)
		panel->funcs->disable(panel);
}

#endif	/* _ROCKCHIP_PANEL_H_ */
