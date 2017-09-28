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
    type = TREATHeat
    variable = Temperature
    center = '0, 0, 0'
    total_energy = 19e9
    transient_duration = 10
    max_temperature = 900
    initial_temperature = 290
  [../]
[]

[Materials]
  [./Fuel] # Essentially graphite, from http://www.azom.com/article.aspx?ArticleID=1630
    type = GenericConstantMaterial
    prop_names =  'thermal_conductivity specific_heat density'
    prop_values = '2.475                0.800         1.8' # W/(cm K), J/(g K), g/cm^3
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
  file_base = test
  exodus = true
  [./csv]
    type = CSV
    file_base = csv/test
  []
[]
