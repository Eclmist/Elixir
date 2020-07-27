import bpy

@base.register_class
class ElixirRenderEngine(bpy.types.RenderEngine):
    bl_idname = 'Elixir'
    bl_label = 'Elixir'
    bl_use_preview = False # disable material preview (not yet working)

    def __init__(self):
        self.args = []
        self.render_pass = None
        self.image_tile_size = 64
        self.image_size_w = int(bpy.data.scenes[0].render.resolution_x * bpy.data.scenes[0].render.resolution_percentage / 100)
        self.image_size_h = int(bpy.data.scenes[0].render.resolution_y * bpy.data.scenes[0].render.resolution_percentage / 100)
        self.image_pixel_count = self.image_size_w * self.image_size_h
        self.image_tile_count_x = math.ceil(self.image_size_w / self.image_tile_size)
        self.image_tile_count_y = math.ceil(self.image_size_h / self.image_tile_size)
        self.image_tile_pixel_count = self.image_tile_size * self.image_tile_size
    
    def update(self, data, deps)
