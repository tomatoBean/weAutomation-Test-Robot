#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xa6ef395c, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x5f93734c, __VMLINUX_SYMBOL_STR(tty_unregister_driver) },
	{ 0x79a1d9da, __VMLINUX_SYMBOL_STR(tty_unregister_device) },
	{ 0xb7d1870a, __VMLINUX_SYMBOL_STR(tty_port_destroy) },
	{ 0x94665bd1, __VMLINUX_SYMBOL_STR(put_tty_driver) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0x81d08578, __VMLINUX_SYMBOL_STR(tty_register_driver) },
	{ 0x74a9a24c, __VMLINUX_SYMBOL_STR(tty_port_link_device) },
	{ 0x285c950f, __VMLINUX_SYMBOL_STR(tty_port_init) },
	{ 0xe4e48905, __VMLINUX_SYMBOL_STR(tty_set_operations) },
	{ 0x67b27ec1, __VMLINUX_SYMBOL_STR(tty_std_termios) },
	{ 0x7d3d32a8, __VMLINUX_SYMBOL_STR(__tty_alloc_driver) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x8526c35a, __VMLINUX_SYMBOL_STR(remove_wait_queue) },
	{ 0x1000e51, __VMLINUX_SYMBOL_STR(schedule) },
	{ 0xc9fef317, __VMLINUX_SYMBOL_STR(add_wait_queue) },
	{ 0xffd5a395, __VMLINUX_SYMBOL_STR(default_wake_function) },
	{ 0x5ff8bca7, __VMLINUX_SYMBOL_STR(current_task) },
	{ 0x4f8b5ddb, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x1f9651ad, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x4b37e7bc, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x32b33f99, __VMLINUX_SYMBOL_STR(tty_flip_buffer_push) },
	{ 0x6f2fe034, __VMLINUX_SYMBOL_STR(tty_insert_flip_string_fixed_flag) },
	{ 0x78e739aa, __VMLINUX_SYMBOL_STR(up) },
	{ 0x6dc6dd56, __VMLINUX_SYMBOL_STR(down) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D2F57EAA6B7DB635F032034");
