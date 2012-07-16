# this file maps RNA to online URL's for right mouse context menu
# documentation access

# URL prefix is the: url_manual_prefix + url_manual_mapping[id]

url_manual_prefix = "http://wiki.blender.org/index.php/Doc:2.6/Manual/"

# - The first item is a wildcard - typical file system globbing
#   using python module 'fnmatch.fnmatch'
# - Expressions are evaluated top down (include catch-all expressions last).

url_manual_mapping = (

    # *** Modifiers ***
    # --- Intro ---
    ("bpy.types.Modifier.show_*", "Modifiers/The_Stack"),
    ("bpy.types.Modifier.*", "Modifiers"),  # catchall for various generic options
    # --- Modify Modifiers ---
    ("bpy.types.UVProjectModifier.*",              "Modifiers/Modify/UV_Project"),
    ("bpy.types.VertexWeightMixModifier.*",        "Modifiers/Modify/Vertex_Weight"),
    ("bpy.types.VertexWeightEditModifier.*",       "Modifiers/Modify/Vertex_Weight"),
    ("bpy.types.VertexWeightProximityModifier.*",  "Modifiers/Modify/Vertex_Weight"),
    # --- Generate Modifiers ---
    ("bpy.types.ArrayModifier.*",      "Modifiers/Generate/Array"),
    ("bpy.types.BevelModifier.*",      "Modifiers/Generate/Bevel"),
    ("bpy.types.BooleanModifier.*",    "Modifiers/Generate/Booleans"),
    ("bpy.types.BuildModifier.*",      "Modifiers/Generate/Build"),
    ("bpy.types.DecimateModifier.*",   "Modifiers/Generate/Decimate"),
    ("bpy.types.EdgeSplitModifier.*",  "Modifiers/Generate/Edge_Split"),
    ("bpy.types.MaskModifier.*",       "Modifiers/Generate/Mask"),
    ("bpy.types.MirrorModifier.*",     "Modifiers/Generate/Mirror"),
    ("bpy.types.MultiresModifier.*",   "Modifiers/Generate/Multiresolution"),
    ("bpy.types.ScrewModifier.*",      "Modifiers/Generate/Screw"),
    ("bpy.types.SkinModifier.*",       "Modifiers/Generate/Skin"),
    ("bpy.types.SolidifyModifier.*",   "Modifiers/Generate/Solidify"),
    ("bpy.types.SubsurfModifier.*",    "Modifiers/Generate/Subsurf"),
    # --- Deform Modifiers ---
    ("bpy.types.ArmatureModifier.*",      "Modifiers/Deform/Armature"),
    ("bpy.types.CastModifier.*",          "Modifiers/Deform/Cast"),
    ("bpy.types.CurveModifier.*",         "Modifiers/Deform/Curve"),
    ("bpy.types.DisplaceModifier.*",      "Modifiers/Deform/Displace"),
    ("bpy.types.DynamicPaintModifier.*",  "Physics/Dynamic_Paint"),
    ("bpy.types.HookModifier.*",          "Modifiers/Deform/Hooks"),
    ("bpy.types.LatticeModifier.*",       "Modifiers/Deform/Lattice"),
    ("bpy.types.MeshDeformModifier.*",    "Modifiers/Deform/Mesh_Deform"),
    ("bpy.types.RemeshModifier.*",        "Modifiers/Deform/"),
    ("bpy.types.ShrinkwrapModifier.*",    "Modifiers/Deform/Shrinkwrap"),
    ("bpy.types.SimpleDeformModifier.*",  "Modifiers/Deform/Simple_Deform"),
    ("bpy.types.SmoothModifier.*",        "Modifiers/Deform/Smooth"),
    # ("bpy.types.SurfaceModifier.*",     "Modifiers/Deform/"),  # USERS NEVER SEE THIS
    ("bpy.types.WarpModifier.*",          "Modifiers/Deform/Warp"),
    ("bpy.types.WaveModifier.*",          "Modifiers/Deform/Wave"),
    # --- Simulate Modifiers ---
    ("bpy.types.ClothModifier.*",             "Physics/Cloth"),
    ("bpy.types.CollisionModifier.*",         "Physics/Collision"),
    ("bpy.types.ExplodeModifier.*",           "Modifiers/Simulate/Explode"),
    ("bpy.types.FluidSimulationModifier.*",   "Physics/Fluid"),
    ("bpy.types.OceanModifier.*",             "Modifiers/Simulate/Ocean"),
    ("bpy.types.ParticleInstanceModifier.*",  "Modifiers/Simulate/Particle_Instance"),
    ("bpy.types.ParticleSystemModifier.*",    "Physics/Particles"),
    ("bpy.types.SmokeModifier.*",             "Physics/Smoke"),
    ("bpy.types.SoftBodyModifier.*",          "Physics/Soft_Body"),

    # *** Constraints ***
    ("bpy.types.Constraint.*", "Constraints"),
    ("bpy.types.Constraint.mute", "Constraints/The_Stack"),  # others could be added here?
    # --- Transform Constraints ---
    ("bpy.types.CopyLocationConstraint.*",    "Constraints/Transform/Copy_Location"),
    ("bpy.types.CopyRotationConstraint.*",    "Constraints/Transform/Copy_Rotation"),
    ("bpy.types.CopyScaleConstraint.*",       "Constraints/Transform/Copy_Scale"),
    ("bpy.types.CopyTransformsConstraint.*",  "Constraints/Transform/Copy_Transforms"),
    ("bpy.types.LimitDistanceConstraint.*",   "Constraints/Transform/Limit_Distance"),
    ("bpy.types.LimitLocationConstraint.*",   "Constraints/Transform/Limit_Location"),
    ("bpy.types.LimitRotationConstraint.*",   "Constraints/Transform/Limit_Rotation"),
    ("bpy.types.LimitScaleConstraint.*",      "Constraints/Transform/Limit_Scale"),
    ("bpy.types.MaintainVolumeConstraint.*",  "Constraints/Transform/Maintain_Volume"),
    ("bpy.types.TransformConstraint.*",       "Constraints/Transform/Transformation"),
    # --- Tracking Constraints ---
    ("bpy.types.ClampToConstraint.*",      "Constraints/Tracking/Clamp_To"),
    ("bpy.types.DampedTrackConstraint.*",  "Constraints/Tracking/Damped_Track"),
    ("bpy.types.KinematicConstraint.*",    "Constraints/Tracking/IK_Solver"),
    ("bpy.types.LockedTrackConstraint.*",  "Constraints/Tracking/Locked_Track"),
    ("bpy.types.SplineIKConstraint.*",     "Constraints/Tracking/Spline_IK"),
    ("bpy.types.StretchToConstraint.*",    "Constraints/Tracking/Stretch_To"),
    ("bpy.types.TrackToConstraint.*",      "Constraints/Tracking/Track_To"),
    # --- Relationship Constraints ---
    ("bpy.types.ActionConstraint.*",          "Constraints/Relationship/Action"),
    ("bpy.types.CameraSolverConstraint.*",    "Motion_Tracking"),  # not exact match
    ("bpy.types.ChildOfConstraint.*",         "Constraints/Relationship/Action"),
    ("bpy.types.FloorConstraint.*",           "Constraints/Relationship/Child_Of"),
    ("bpy.types.FollowPathConstraint.*",      "Constraints/Relationship/Floor"),
    ("bpy.types.FollowTrackConstraint.*",     "Constraints/Relationship/Follow_Path"),
    ("bpy.types.ObjectSolverConstraint.*",    "Motion_Tracking"),  # not exact match
    ("bpy.types.PivotConstraint.*",           "Constraints/Relationship/Pivot"),
    ("bpy.types.PythonConstraint.*",          "Constraints/Relationship/Script"),
    ("bpy.types.RigidBodyJointConstraint.*",  "Constraints/Relationship/Rigid_Body_Joint"),
    ("bpy.types.ShrinkwrapConstraint.*",      "Constraints/Relationship/Shrinkwrap"),

    ("bpy.types.ImageFormatSettings.*",  "Render/Output#File_Type"),
    ("bpy.types.RenderSettings.filepath",  "Render/Output#File_Locations"),
    ("bpy.types.RenderSettings.*",       "Render"),  # catchall, TODO - refine

    # *** ID Subclasses ***
    ("bpy.types.Action.*", "Animation/Actions"),
    #("bpy.types.Brush.*", ""),  # TODO - manual has no place for this! XXX
    ("bpy.types.Curve.*", "Modeling/Curves"),
    ("bpy.types.GreasePencil.*", "3D_interaction/Sketching/Drawing"),
    ("bpy.types.Group.*", "Modeling/Objects/Groups_and_Parenting#Grouping_objects"),
    ("bpy.types.Image.*", "Textures/Types/Image"),
    ("bpy.types.ShapeKey.*", "Animation/Basic/Deformation/Shape_Keys"), # not an ID but include because of Key
    ("bpy.types.Key.*", "Animation/Basic/Deformation/Shape_Keys"),
    #("bpy.types.Lattice.*", ""), # TODO - manual has no place for this! XXX
    ("bpy.types.Library.*", "Manual/Data_System/Linked_Libraries"),
    #("bpy.types.Mask.*", ""), # TODO - manual has no place for this! XXX
    # *** Materials (blender internal) ***
    ("bpy.types.Material.diffuse*", "Materials/Properties/Diffuse_Shaders"),
    ("bpy.types.Material.*", "Materials"),  # catchall, until the section is filled in
    # ... todo, many more options
    ("bpy.types.MovieClip.*", "Motion_Tracking#Movie_Clip_Editor"),
    #("bpy.types.NodeTree.*", ""),  # dont document
    ("bpy.types.Object.*",  "Modeling/Objects"),  # catchall, TODO - refine
    ("bpy.types.ParticleSettings.*", "Physics/Particles"),
    ("bpy.types.Scene.*", "Interface/Scenes"),
    ("bpy.types.Screen.*", "Interface/Screens"),
    #("bpy.types.Sound.*", ""), # TODO - manual has no place for this! XXX
    #("bpy.types.Speaker.*", ""), # TODO - manual has no place for this! XXX
    ("bpy.types.Text.*", "Extensions/Python/Text_editor"),
    ("bpy.types.Texture.*", "Textures"),
    ("bpy.types.VectorFont.*", "Modeling/Texts"),
    ("bpy.types.WindowManager.*", "Interface/Window_system"),
    ("bpy.types.World.*", "World"),

    # *** ID Subclasses (cont.) Object Data ***
    ("bpy.types.Mesh.*",  "Modeling/Meshes"),  # catchall, TODO - refine
    ("bpy.types.MetaBall.*",  "Modeling/Metas"),  # catchall, TODO - refine
    ("bpy.types.TextCurve.*",  "Modeling/Texts"),  # catchall, TODO - refine
    ("bpy.types.Armature.*",  "Rigging/Armatures"),  # catchall, TODO - refine
    ("bpy.types.Camera.*",  "Render/Camera"),  # catchall, TODO - refine

    ("bpy.types.PointLamp.*",  "Lighting/Lamps/Lamp"),  # catchall, TODO - refine
    ("bpy.types.AreaLamp.*",   "Lighting/Lamps/Area"),  # catchall, TODO - refine
    ("bpy.types.SpotLamp.*",   "Lighting/Lamps/Spot"),  # catchall, TODO - refine
    ("bpy.types.SunLamp.*",    "Lighting/Lamps/Sun"),   # catchall, TODO - refine
    ("bpy.types.HemiLamp.*",   "Lighting/Lamps/Hemi"),  # catchall, TODO - refine
    ("bpy.types.Lamp.*",       "Lighting"),             # catchall, TODO - refine

    # --- Animation ---
    ("bpy.types.Keyframe.*", "Animation/Keyframes"),
    ("bpy.types.FCurve.*", "Animation/Editors/Graph/FCurves"),
    
    # --- Rigging ---
    ("bpy.types.Bone.*",      "Armatures/Bones"),
    ("bpy.types.EditBone.*",  "Armatures/Bones"),
    ("bpy.types.PoseBone.*",  "Rigging/Posing"),

    # --- World ---
    ("bpy.types.World.*",  "World"),

    ("bpy.types.Texture.*",  "Textures"),

    # === Operators ===
    # Catch all only for now!
    # *** Window/Screen ***
    
    ("bpy.ops.action.*",  "Animation/Actions"),
    ("bpy.ops.anim.*",  "Animation"),
    ("bpy.ops.armature.*",  "Rigging/Armatures"),
    ("bpy.ops.boid.*",  "Physics/Particles/Physics/Boids"),
    # ("bpy.ops.brush.*",  ""),  # TODO
    ("bpy.ops.buttons.*",  "Interface/Buttons_and_Controls"),
    ("bpy.ops.camera.*",  "Render/Camera"),
    ("bpy.ops.clip.*",  "Motion_Tracking#Movie_Clip_Editor"),
    ("bpy.ops.cloth.*",  "Physics/Cloth"),
    ("bpy.ops.console.*",  "Python/Console"),
    ("bpy.ops.constraint.*",  "Constraints"),
    ("bpy.ops.curve.*",  "Modeling/Curves"),
    ("bpy.ops.dpaint.*",  "Physics/Dynamic_Paint"),
    # ("bpy.ops.ed.*",  ""),  # TODO, this is for internal use only?
    # ("bpy.ops.export_anim.*",  ""),  # TODO
    # ("bpy.ops.export_mesh.*",  ""),  # TODO
    # ("bpy.ops.export_scene.*",  ""),  # TODO
    ("bpy.ops.file.*",  ""),
    ("bpy.ops.fluid.*",  "Physics/Fluid"),
    ("bpy.ops.font.*",  "Modeling/Texts"),
    ("bpy.ops.gpencil.*",  "3D_interaction/Sketching/Drawing"),
    ("bpy.ops.graph.*",  "Animation/Editors/Graph/FCurves"),
    ("bpy.ops.group.*",  "Modeling/Objects/Groups_and_Parenting#Grouping_objects"),
    ("bpy.ops.image.*",  "Textures/Types/Image"),
    # ("bpy.ops.import_anim.*",  ""),  # TODO
    # ("bpy.ops.import_curve.*",  ""),  # TODO
    # ("bpy.ops.import_mesh.*",  ""),  # TODO
    # ("bpy.ops.import_scene.*",  ""),  # TODO
    # ("bpy.ops.info.*",  ""),  # TODO
    ("bpy.ops.lamp.*",  "Lighting"),  # --- TODO ... all below ---
    # ("bpy.ops.lattice.*",  ""),  # TODO
    ("bpy.ops.logic.*",  "Game_Engine/Logic"),
    ("bpy.ops.marker.*",  "Animation/Markers"),
    # ("bpy.ops.mask.*",  ""),  # TODO
    ("bpy.ops.material.*",  "Materials"),
    ("bpy.ops.mball.*",  "Modeling/Metas"),
    ("bpy.ops.mesh.*",  "Modeling/Meshes"),
    ("bpy.ops.nla.*",  "Animation/Editors/NLA"),
    # ("bpy.ops.node.*",  ""),  # TODO
    ("bpy.ops.object.*",  "Modeling/Objects"),
    ("bpy.ops.outliner.*",  "Data_System/The_Outliner"),
    # ("bpy.ops.paint.*",  ""),  # TODO
    ("bpy.ops.particle.*",  "Physics/Particles"),
    ("bpy.ops.pose.*",  "Rigging/Posing"),
    ("bpy.ops.poselib.*",  "Rigging/Posing/Pose_Library"),
    # ("bpy.ops.ptcache.*",  ""),  # TODO
    ("bpy.ops.render.*",  "Render"),
    ("bpy.ops.scene.*",  "Interface/Scenes"),
    ("bpy.ops.screen.*",  "Interface/Window_system"),
    ("bpy.ops.script.*",  "Extensions/Python"),
    ("bpy.ops.sculpt.*",  "Modeling/Meshes/Editing/Sculpt_Mode"),
    ("bpy.ops.sequencer.*",  "Sequencer/Usage"),
    # ("bpy.ops.sketch.*",  ""),  # TODO
    # ("bpy.ops.sound.*",  ""),  # TODO
    ("bpy.ops.surface.*",  "Modeling/Surfaces"),
    ("bpy.ops.text.*",  "Extensions/Python/Text_editor"),
    ("bpy.ops.texture.*",  "Textures"),
    ("bpy.ops.time.*",  "Animation/Timeline"),
    ("bpy.ops.transform.*",  "3D_interaction/Transform_Control"),
    ("bpy.ops.ui.*",  "Interface"),
    ("bpy.ops.uv.*",  "Textures/Mapping/UV/Layout_Editing"),
    # ("bpy.ops.view2d.*",  ""),  # TODO
    ("bpy.ops.view3d.*",  "3D_interaction/Navigating/3D_View"),  # this link is a bit arbitrary
    ("bpy.ops.wm.*",      "Interface/Window_system"),
    ("bpy.ops.world.*",  "World"),
)

# may have 'url_reference_mapping'... etc later