# This is a 'fuel pin' scenario, adapted from a cylindrical model available in MOOSE's ex06

[Mesh]
  type = FileMesh
  file = cylinder.e
  uniform_refine = 1
[]

[Variables]
  [./NonCopyTransfersOnlyWorkWithAuxVariables]
  [../]
[]

[AuxVariables]
  [./Heat]
    order = FIRST
    family = LAGRANGE
  [../]
  [./Temperature]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  [./SoWeHaveToDefineAKernelAndVariableThatDoNothing]
    type = NullKernel
    variable = NonCopyTransfersOnlyWorkWithAuxVariables
  [../]
[]

[AuxKernels]
  [./HeatGeneration]
    type = FuelPinHeatAux
    in = Temperature
    variable = Heat
    center = '0.15, 0.15, -0.2'
    total_energy = 14e3
    max_temperature = 600
    initial_temperature = 290
  [../]
  [./TemperatureConversion]
    type = FunctionSeriesToAux
    function = FE_Basis
    variable = Temperature
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

[UserObjects]
  [./HeatGenerationFE]
    type = FEVolumeUserObject
    function = FE_Basis
    variable = Heat
  [../]
[]
