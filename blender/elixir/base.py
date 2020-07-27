import bpy

# a global container is used to keep all lambda function to register classes
REGISTRARS = []
def registrar(register, unregister, name=None):
    global REGISTRARS
    if name is None or not [True for _, _, n in REGISTRARS if n == name]:
        REGISTRARS.append((register, unregister, name))

# register a class in blender system, this function just
def register_class(cls):
    registrar(lambda: bpy.utils.register_class(cls), lambda: bpy.utils.unregister_class(cls), cls.__name__)
    return cls

# register some internal SORT compatible panels
def get_sort_compatible_panels():
    def is_panel_compatible(panel):
        compatible_panels = {
            'RENDER_PT_dimensions',
            'DATA_PT_lens',
            'DATA_PT_camera',
        }
        return hasattr(panel, 'COMPAT_ENGINES') and 'BLENDER_RENDER' in panel.COMPAT_ENGINES and panel.__name__ in compatible_panels
    return [panel for panel in bpy.types.Panel.__subclasses__() if is_panel_compatible(panel)]

# trigger the real registering of all lambda function in the container
def register():
    for r, _, _ in REGISTRARS:
        r()

    for t in get_sort_compatible_panels():
        t.COMPAT_ENGINES.add('Elixir')

# unregister everything already registered
def unregister():
    for t in get_sort_compatible_panels():
        t.COMPAT_ENGINES.remove('Elixir')

    for _, u, _ in reversed(REGISTRARS):
        u()
