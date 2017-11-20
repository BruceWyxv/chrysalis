# This is a simplistic simulation of a single fuel rod in TREAT. Many of the parameters were taken from:
# http://www.iaea.org/inis/collection/NCLCollectionStore/_Public/12/627/12627688.pdf
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 6
  xmin = 0
  xmax = 0.4
  ny = 80
  ymin = 0
  ymax = 10
[]

[Variables]
  [./fuel_temp]
  [../]
[]

[Kernels]
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
  [../]
[]

[ICs]
  [./start_fuel_temp]
    type = ConstantIC
    value = 290
    variable = fuel_temp
  [../]
[]

[BCs]
  [./no_heat_flux_across_fuel_center]
    type = NeumannBC
    variable = fuel_temp
    boundary = left
    value = 0
  [../]
  [./interface_temperature]
    type = FEValueBC
    variable = fuel_temp
    boundary = right
    function = FE_Basis_Temperature
  [../]
  [./interface_flux]
    type = FEFluxBC
    boundary = right
    variable = fuel_temp
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
    variable = fuel_temp
    boundary = right
  [../]
  [./FEHeat]
    type = FEBoundaryFluxUserObject
    function = FE_Basis_Heat
    variable = fuel_temp
    boundary = right
    diffusion_coefficient = thermal_conductivity
  [../]
[]

[MultiApps]
  [./FETransferApp]
    type = TransientMultiApp
    input_files = water.i
    sub_cycling = true
    output_sub_cycles = true
  [../]
[]

[Transfers]
  #[./TemperatureToSub]
  #  type = MultiAppFETransfer
  #  direction = to_multiapp
  #  multi_app = FETransferApp
  #  this_app_object_name = FETemperature
  #  multi_app_object_name = FE_Basis_Temperature
  #[../]
  [./HeatToSub]
    type = MultiAppFETransfer
    direction = to_multiapp
    multi_app = FETransferApp
    this_app_object_name = FEHeat
    multi_app_object_name = FE_Basis_Heat
  [../]
  [./TemperatureToMe]
    type = MultiAppFETransfer
    direction = from_multiapp
    multi_app = FETransferApp
    this_app_object_name = FE_Basis_Temperature
    multi_app_object_name = FETemperature
  [../]
  [./HeatToMe]
    type = MultiAppFETransfer
    direction = from_multiapp
    multi_app = FETransferApp
    this_app_object_name = FE_Basis_Heat
    multi_app_object_name = FEHeat
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
  picard_max_its = 30
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-9
  picard_rel_tol = 1e-8
  picard_abs_tol = 1e-9
[]

[Postprocessors]
  [./average_interface_temperature]
    type = SideAverageValue
    variable = fuel_temp
    boundary = right
  [../]
  [./total_flux]
    type = SideFluxIntegral
    variable = fuel_temp
    boundary = right
    diffusivity = 'thermal_conductivity'
  [../]
[]

[Outputs]
  exodus = true
  file_base = test_fuel
  [./csv]
    type = CSV
    file_base = csv/test
  []
[]
