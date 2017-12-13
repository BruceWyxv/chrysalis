# This is a 'fuel pin' scenario, adapted from a cylindrical model available in MOOSE's ex06

[Mesh]
  type = FileMesh
  file = cylinder.e
  uniform_refine = 2
[]

[Variables]
  [./Temperature]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[ICs]
  [./T_IC]
    type = ConstantIC
    variable = Temperature
    value = 290
  [../]
[]

[BCs]
  [./T_BC]
    type = NeumannBC
    variable = Temperature
    boundary = outside
    value = -10
  [../]
[]

[Kernels]
  [./HeatDiff]
    type = HeatConduction
    variable = Temperature
  [../]
  [./HeadDiffTime]
    type = HeatConductionTimeDerivative
    variable = Temperature
  [../]
  [./HeatGeneration]
    type = FuelPinHeat
    variable = Temperature
    center = '0.15, 0.15, -0.2'
    total_energy = 14e3
    max_temperature = 600
    initial_temperature = 290
  [../]
[]

[Materials]
  [./Fuel] # Essentially UO2
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.053                0.233         10.5' # W/(cm K), J/(g K), g/cm^3
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 80
  dt = 0.5
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Postprocessors]
  [./Elapsed_Time]
    type = PerformanceData
    event = ALIVE
    outputs = 'console csv'
  [../]
  [./Iterations:_Linear]
    type = NumLinearIterations
    outputs = 'csv'
  [../]
  [./Iterations:_Nonlinear]
    type = NumNonlinearIterations
    outputs = 'csv'
  [../]
  [./Iterations:_Picard]
    type = NumPicardIterations
    outputs = 'csv'
  [../]
  [./Temperature:_Average]
    type = ElementAverageValue
    variable = Temperature
  [../]
  [./Temperature:_Peak]
    type = ElementExtremeValue
    value_type = max
    variable = Temperature
  [../]
[]

[Outputs]
  file_base = test_refine
  exodus = true
  [./csv]
    type = CSV
    file_base = csv/test_refine
  []
[]
