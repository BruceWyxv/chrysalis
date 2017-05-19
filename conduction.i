[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 30
  ny = 2
  nz = 2
  xmin = 0
  xmax = 10
  ymin = 0
  ymax = 1
  zmin = 0
  zmax = 1
[]

[Variables]
  [./T]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./T_IC]
    type = FunctionIC
    variable = T
    function = '300+20*x'
  [../]
[]

[Kernels]
  active = 'HeatDiff'
  [./HeatDiff]
    type = HeatConduction
    variable = T
  [../]
  [./HeatTdot]
    type = HeatConductionTimeDerivative
    variable = T
  [../]
  [./HeatSourceConstant]
    type = HeatSource
    variable = T
    function = '0.2' #W/cm^3
  [../]
  [./HeatSourceTransient]
    type = HeatSource
    variable = T
    function = '0.2*(1-100/(t+100))' #W/cm^3
  [../]
[]

[BCs]
  active = 'left right'
  [./left]
    type = DirichletBC
    variable = T
    boundary = left
    value = 300
  [../]
  [./right]
    type = FunctionDirichletBC
    variable = T
    boundary = right
    function = '300+200*exp(-0.004*t)'
  [../]
[]

[Materials]
  [./k]
    type = GenericConstantMaterial
    prop_names = thermal_conductivity
    prop_values = 0.0759 # UO2 in W/(cm K)
    block = 0
  [../]
  [./cp]
    type = GenericConstantMaterial
    prop_names = specific_heat
    prop_values = 0.235 # UO2 in J/(g K)
    block = 0
  [../]
  [./rho]
    type = GenericConstantMaterial
    prop_names = density
    prop_values = 10.9 # UO2 in g/(cm^3)
    block = 0
  [../]
[]

[Executioner]
  type = Transient
  solve_type = 'PJFNK'
  #scheme = bdf2
  dt = 5
  num_steps = 2
  #petsc_options_iname = '-pc_type -pc_hypre_type'
  #petsc_options_value = 'hypre boomeramg'
[]

[UserObjects]
  [./functional_expansion]
    type = FECoefficientsUserObject
    functional = Cartesian
    orders = '4'
    valid_range = '0 10'
    orders = '4 1 1'
    valid_range = '0 10 0 1 0 1'
    variable = T
    execute_on = timestep_begin
    print_state = true
  [../]
[]

[Outputs]
  file_base =
  exodus = true
[]
