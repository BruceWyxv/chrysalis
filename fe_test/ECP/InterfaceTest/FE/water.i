# This is a simplistic simulation of a single fuel rod in TREAT. Many of the parameters were taken from:
# http://www.iaea.org/inis/collection/NCLCollectionStore/_Public/12/627/12627688.pdf
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 30
  xmin = 0.4
  xmax = 2.4
  ny = 80
  ymin = 0
  ymax = 10
[]

[Variables]
  [./water_temp]
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
[]

[Materials]
  [./Water]
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '0.591                4.182         0.9983' # W/(cm K), J/(g K), g/cm^3
    block = 0
  [../]
[]

[ICs]
  [./start_water_temp]
    type = ConstantIC
    value = 290
    variable = water_temp
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
  [./heat_from_fuel]
    type = FEFluxBC
    boundary = left
    variable = water_temp
    function = FE_Basis_Heat
  [../]
[]

[Functions]
  [./FE_Basis_Temperature]
    type = FunctionSeries
    series_type = Cartesian
    orders = '5'
    physical_bounds = '0.0 10'
    y = Legendre
  [../]
  [./FE_Basis_Heat]
    type = FunctionSeries
    series_type = Cartesian
    orders = '5'
    physical_bounds = '0.0 10'
    y = Legendre
  [../]
[]

[UserObjects]
  [./FETemperature]
    type = FEBoundaryValueUserObject
    function = FE_Basis_Temperature
    variable = water_temp
    boundary = left
  [../]
  [./FEHeat]
    type = FEBoundaryFluxUserObject
    function = FE_Basis_Heat
    variable = water_temp
    boundary = left
    diffusivity = thermal_conductivity
  [../]
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
[]


[Outputs]
  exodus = true
  file_base = test_water
[]
