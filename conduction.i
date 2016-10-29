[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 4
  ny = 40
  nz = 4
  xmin = 0
  xmax = 1
  ymin = 0
  ymax = 10
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
    type = ConstantIC
    variable = T
    value = 300
  [../]
[]

[Kernels]
  active = 'HeatDiff HeatSourceConstant'
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
  active = 'bottom'
  [./bottom]
    type = DirichletBC
    variable = T
    boundary = bottom
    value = 300
  [../]
  [./top]
    type = FunctionDirichletBC
    variable = T
    boundary = top
    function = '300+200*(t/2000)^0.25'
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
  type = Steady
  solve_type = 'PJFNK'
  #scheme = bdf2
  #dt = 25
  #num_steps = 1000
  #petsc_options_iname = '-pc_type -pc_hypre_type'
  #petsc_options_value = 'hypre boomeramg'
[]

[Outputs]
  file_base = steadystate
  exodus = true
[]
