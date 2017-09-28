# This is a simplistic simulation of a single fuel rod in TREAT. Many of the parameters were taken from:
# http://www.iaea.org/inis/collection/NCLCollectionStore/_Public/12/627/12627688.pdf
[Mesh]
  type = GeneratedMesh
  dim = 3
  nx = 256
  ny = 32
  nz = 32
  xmin = -60
  xmax = 60
  ymin = -5
  ymax = 5
  zmin = -5
  zmax = 5
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
    type = TREATHeatAux
    in = Temperature
    variable = Heat
    center = '0, 0, 0'
    total_energy = 19e9
    transient_duration = 10
    max_temperature = 900
    initial_temperature = 290
  [../]
  [./TemperatueConversion]
    type = FunctionAux
    execute_on = timestep_begin
    function = FE_Basis
    variable = Temperature
  [../]
[]

[Executioner]
  type = Transient
  num_steps = 40
  dt = 0.5
  solve_type = PJFNK
  petsc_options_iname = '-pc_type -pc_hypre_type'
  petsc_options_value = 'hypre boomeramg'
[]

[Functions]
  [./FE_Basis]
    type = FunctionSeries
    series_type = Cartesian
    orders = '5, 3, 3'
    physical_bounds = '-60 60 -5 5 -5 5'
    x = Legendre
    y = Legendre
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
