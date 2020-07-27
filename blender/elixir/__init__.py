bl_info = {
    "name": "Elixir Render Engine",
    "author": "Samuel Van Allen",
    "version": (0, 1, 0),
    "blender": (2, 80, 0),
    "location": "Info > RenderEngine",
    "description": "An open-source physically based pathtracer",
    "warning": "Elixir is currently a WIP and is considered unstable. Proceed with caution at your own risk.",
    "wiki_url": "https://github.com/Eclmist/Elixir",
    "category": "Render",
}

import bpy
from . import base
from . import elixir

@base.register_class
class ElixirAddonPreferences(bpy.types.AddonPreferences):
    bl_idname = __package__
    install_path : bpy.props.StringProperty( name="Path to Elixir binary", description='Path to Elixir binary', subtype='DIR_PATH', default='',)
    def draw(self, context):
        self.layout.prop(self, "install_path")
        
def register():
    # Register all classes tagged with @base.register_class in this plugin
    base.register()
    
def unregister():
    base.unregister()
