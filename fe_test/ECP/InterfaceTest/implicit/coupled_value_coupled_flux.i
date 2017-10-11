[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 20
  xmax = 2
  ny = 20
  ymax = 2
[]

[MeshModifiers]
  [./subdomain1]
    type = SubdomainBoundingBox
    bottom_left = '0 0 0'
    top_right = '1 1 0'
    block_id = 1
  [../]
  [./interface]
    type = SideSetsBetweenSubdomains
    depends_on = subdomain1
    master_block = '0'
    paired_block = '1'
    new_boundary = 'master0_interface'
  [../]
  [./break_boundary]
    depends_on = interface
    type = BreakBoundaryOnSubdomain
  [../]
[]

[Variables]
  [./u]
    order = FIRST
    family = LAGRANGE
    block = 0
  [../]

  [./v]
    order = FIRST
    family = LAGRANGE
    block = 1
  [../]
[]

[Kernels]
  [./diff_u]
    type = HeatConduction
    variable = u
  [../]
  [./diff_v]
    type = HeatConduction
    variable = v
  [../]
  [./source_u]
    type = BodyForce
    variable = u
    value = 1
  [../]
[]

[InterfaceKernels]
  [./couple_flux]
    type = InterfaceDiffusion
    variable = u
    neighbor_var = v
    boundary = master0_interface
    variable_side_diffusivity = thermal_conductivity
  [../]
[]

[Materials]
  [./Fuel] # Essentially graphite, from http://www.azom.com/article.aspx?ArticleID=1630
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '2.475                0.800         1.8' # W/(cm K), J/(g K), g/cm^3
    block = 0
  [../]
  [./Water]
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.591                4.182         0.9983' # W/(cm K), J/(g K), g/cm^3
    block = 1
  [../]
[]

[BCs]
  [./u]
    type = VacuumBC
    variable = u
    boundary = 'left_to_0 bottom_to_0 right top'
  [../]
  [./v]
    type = VacuumBC
    variable = v
    boundary = 'left_to_1 bottom_to_1'
  [../]
  [./couple_value]
    type = MatchedValueBC
    variable = v
    boundary = 'master0_interface'
    v = u
  [../]
[]

[Postprocessors]
  [./u_int]
    type = ElementIntegralVariablePostprocessor
    variable = u
    block = 0
  [../]
  [./v_int]
    type = ElementIntegralVariablePostprocessor
    variable = v
    block = 1
  [../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON
[]

[Outputs]
  exodus = true
  print_linear_residuals = true
[]
