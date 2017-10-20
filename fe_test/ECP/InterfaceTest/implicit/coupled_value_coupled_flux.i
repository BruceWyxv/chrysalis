[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 36
  xmin = 0
  xmax = 2.4
  ny = 80
  ymin = 0
  ymax = 10
[]

[MeshModifiers]
  [./subdomain1]
    type = SubdomainBoundingBox
    bottom_left = '0.0 0.0 0.0'
    top_right = '0.4 10.0 0.0'
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
  [./water_temp]
    block = 0
  [../]

  [./fuel_temp]
    block = 1
  [../]
[]

[Kernels]
  [./diff_water_temp]
    type = HeatConduction
    variable = water_temp
  [../]
  [./time_diff_water_temp]
    type = HeatConductionTimeDerivative
    variable = water_temp
  [../]
  [./diff_fuel_temp]
    type = HeatConduction
    variable = fuel_temp
  [../]
  [./time_diff_fuel_temp]
    type = HeatConductionTimeDerivative
    variable = fuel_temp
  [../]
  [./source_fuel_temp]
    type = BodyForce
    variable = fuel_temp
    value = 300
  [../]
[]

[Materials]
  [./Fuel] # Essentially UO2
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.053                0.233         10.5' # W/(cm K), J/(g K), g/cm^3
    block = 1
  [../]
  [./Water]
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.591                4.182         0.9983' # W/(cm K), J/(g K), g/cm^3
    block = 0
  [../]
[]

[ICs]
  [./start_fuel_temp]
    type = ConstantIC
    value = 290
    variable = fuel_temp
  [../]
  [./start_water_temp]
    type = ConstantIC
    value = 290
    variable = water_temp
  [../]
[]

[InterfaceKernels]
  [./match_water_to_fuel_heat_flux]
    type = InterfaceDiffusion
    variable = water_temp
    neighbor_var = fuel_temp
    boundary = master0_interface
    variable_side_diffusivity = thermal_conductivity
    neighbor_side_diffusivity = thermal_conductivity
  [../]
[]

[BCs]
  [./cool_water_in]
    type = DirichletBC
    variable = water_temp
    boundary = bottom
    value = 290
  [../]
  [./no_heat_flux_across_water_centerline]
    type = NeumannBC
    variable = water_temp
    boundary = right
    value = 0
  [../]
  [./hold_centerline_water_temp]
    type = FunctionDirichletBC
    variable = water_temp
    boundary = right
    function = '290 + y*10'
  [../]
  [./no_heat_flux_across_fuel_center]
    type = NeumannBC
    variable = fuel_temp
    boundary = left
    value = 0
  [../]
  [./match_water_to_fuel_temperatures]
    type = MatchedValueBC
    variable = fuel_temp
    boundary = 'master0_interface'
    v = water_temp
  [../]
[]

[Postprocessors]
  [./average_interface_temperature]
    type = SideAverageValue
    variable = water_temp
    boundary = master0_interface
  [../]
  [./total_flux]
    type = SideFluxIntegral
    variable = water_temp
    boundary = master0_interface
    diffusivity = 'thermal_conductivity'
  [../]
  #[./average_water_temp]
  #  type = ElementAverageValue
  #  variable = water_temp
  #  block = 0
  #[../]
  #[./average_fuel_temp]
  #  type = ElementAverageValue
  #  variable = fuel_temp
  #  block = 1
  #[../]
[]

[Preconditioning]
  [./smp]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 70
  dt = 2.0
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
[]

[Outputs]
  exodus = true
  file_base = implicit
  #print_linear_residuals = true
  [./csv]
    type = CSV
    file_base = csv/test
  []
[]
