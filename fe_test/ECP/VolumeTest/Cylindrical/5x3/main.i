# This is a 'fuel pin' scenario, adapted from a cylindrical model available in MOOSE's ex06

[Mesh]
  type = FileMesh
  file = cylinder.e
  uniform_refine = 1
[]

[Variables]
  [./Temperature]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./Heat]
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
  [./HeatIn]
    type = CoupledForce
    variable = Temperature
    v = Heat
  [../]
[]

[AuxKernels]
  [./ReconstructHeat]
    type = FunctionSeriesToAux
    variable = Heat
    function = FE_Basis
  [../]
[]

[Materials]
  [./Fuel] # Essentially UO2
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.053                0.233         10.5' # W/(cm K), J/(g K), g/cm^3
  [../]
[]

[Functions]
  [./FE_Basis]
    type = FunctionSeries
    series_type = CylindricalDuo
    orders = '5   3'
    physical_bounds = '-2.5 2.5   0 0 1'
    disc = Zernike
    z = Legendre
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 80
  dt = 0.5
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
  picard_max_its = 60
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-9
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

[MultiApps]
  [./FETransferApp]
    type = TransientMultiApp
    input_files = sub.i
    sub_cycling = true
  [../]
[]

[UserObjects]
  [./TemperatureFE]
    type = FEVolumeUserObject
    function = FE_Basis
    variable = Temperature
  [../]
[]

[Transfers]
  [./TemperatureToSub]
    type = MultiAppFETransfer
    direction = to_multiapp
    multi_app = FETransferApp
    multi_app_object_name = FE_Basis
    this_app_object_name = TemperatureFE
  [../]
  [./HeatToMe]
    type = MultiAppFETransfer
    direction = from_multiapp
    multi_app = FETransferApp
    multi_app_object_name = HeatGenerationFE
    this_app_object_name = FE_Basis
  [../]
[]

[Outputs]
  file_base = test
  exodus = true
  [./csv]
    type = CSV
    file_base = csv/test
  []
[]
