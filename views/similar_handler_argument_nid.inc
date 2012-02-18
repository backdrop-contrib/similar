<?php

/**
 * @file
 * Defines the Similar entries node ID argument.
 */

/**
 * Defines the similar entries View node ID argument.
 */
class similar_handler_argument_nid extends views_handler_argument_numeric {

  /**
   * Defines default values for argument settings.
   */
  public function option_definition() {
    $common_words = array('-not', '-and', '-a', '-or', '-the', '-but', '-therefore', '-because', '-can\'t', '-can', '-for',
      '-until', '-the', '-of', '-and', '-to', '-in', '-is', '-you', '-that', '-it', '-he', '-was', '-on', '-are', '-as', '-with',
      '-his', '-they', '-I', '-at', '-be', '-this', '-have', '-from', '-one', '-had', '-by', '-word', '-but', '-not', '-what',
      '-all', '-were', '-we', '-when', '-your', '-said', '-there', '-use', '-an', '-each', '-which', '-she', '-do', '-how',
      '-their', '-if', '-will', '-up', '-other', '-about', '-out', '-many', '-then', '-them', '-these', '-so', '-some', '-her',
      '-would', '-make', '-like', '-him', '-into', '-time', '-has', '-look', '-two', '-more', '-write', '-go', '-see', '-no',
      '-way', '-could', '-people', '-my', '-than', '-first', '-water', '-been', '-call', '-who', '-oil', '-its', '-now', '-find');

    $options = parent::option_definition();
    $options['default_action'] = array('default' => 'default');
    $options['default_argument_type'] = array('default' => 'node');
    $options['validate']['contains']['type'] = array('default' => 'node');
    $options['boolean_mode'] = array('default' => 0);
    $options['enable_custom_operators'] = array('default' => 0);
    $options['custom_operators'] = array('default' => implode(' | ', $common_words));
    $options['increase_title'] = array('default' => 1);
    $options['title_operator'] = array('default' => 'increase');
    $options['include_fields'] = array('default' => 0);
    $options['included_fields'] = array('default' => array());
    return $options;
  }

  /**
   * Defines the options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['not']);
    $form['default_argument_type']['#disabled'] = TRUE;

    $form['similar'] = array(
      '#type' => 'fieldset',
      '#title' => t('FULLTEXT search options'),
    );
    $form['boolean_mode'] = array(
      '#type' => 'checkbox',
      '#title' => t('Execute search in boolean mode'),
      '#default_value' => !empty($this->options['boolean_mode']),
      '#fieldset' => 'similar',
      '#description' => t('Boolean mode enables the ability to increase or decrease the relevance of certain fields.<br />'.
                          'Note: Boolean mode may negatively affect the accuracy of view results.'),
    );
    $form['increase_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Adjust relevance of node title'),
      '#default_value' => !empty($this->options['increase_title']),
      '#description' => t('Increase the relevance of words in the title of the node being viewed.'),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-boolean-mode' => array('1')),
    );
    $form['title_operator'] = array(
      '#type' => 'select',
      '#title' => t('Title relevance'),
      '#options' => array(
        'none' => t('No change'),
        'increase' => t('Increase title relevance'),
        'decrease' => t('Decrease title relevance'),
        'require_all' => t('Require all terms'),
        'require_some' => t('Require at least one term'),
        'require_complete' => t('Require complete title'),
        'exclude_all' => t('Exclude all terms'),
        'exclude_some' => t('Exclude at least one term'),
        'exclude_complete' => t('Exclude complete title'),
      ),
      '#default_value' => $this->options['title_operator'],
      '#description' => t('Select an operator to apply to the title of the current node in matching.'),
      '#fieldset' => 'similar',
      '#dependency' => array(
        'edit-options-boolean-mode' => array('1'),
        'edit-options-increase-title' => array('1'),
      ),
      '#dependency_count' => 2,
    );
    $form['enable_custom_operators'] = array(
      '#type' => 'checkbox',
      '#title' => t('Set custom operators in words and phrases'),
      '#default_value' => !empty($this->options['enable_custom_operators']),
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-boolean-mode' => array('1')),
    );
    $form['custom_operators'] = array(
      '#type' => 'textfield',
      '#title' => t('Operators'),
      '#default_value' => $this->options['custom_operators'],
      '#description' => t('Use boolean logical operators to customize the FULLTEXT query. Each expression should '.
                          'be separated by a vertical bar |.<br />'.
                          'Available operators:  +  -  >  <  (  )  ~  "<br />'.
                          'See http://dev.mysql.com/doc/refman/5.5/en/fulltext-boolean.html for more information.'),
      '#maxlength' => 1024,
      '#fieldset' => 'similar',
      '#dependency' => array('edit-options-boolean-mode' => array('1'), 'edit-options-enable-custom-operators' => array('1')),
      '#dependency_count' => 2,
    );

    if (module_exists('field')) {
      $field_options = array();
      $indices = similar_get_indices();
      foreach (field_info_fields() as $field => $info) {
        if ($info['type'] == 'text') {
          $table = key($info['storage']['details']['sql'][FIELD_LOAD_CURRENT]);
          if (isset($indices[$table])) {
            $field_options[$table] = $field;
          }
        }
      }

      $form['include_fields'] = array(
        '#type' => 'checkbox',
        '#title' => t('Include content fields in matching'),
        '#default_value' => !empty($this->options['include_fields']) ? $this->options['include_fields'] : 0,
        '#description' => t('Enable FULLTEXT queries on fields defined with Field module.'),
        '#fieldset' => 'similar',
      );
      $form['included_fields'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Included fields'),
        '#options' => $field_options,
        '#default_value' => $this->options['included_fields'],
        '#description' => t('Note: Disabling all checkboxes will include all available fields in the query.'),
        '#fieldset' => 'similar',
        '#dependency' => array('edit-options-include-fields' => array('1')),
      );
    }
  }

  /**
   * Overrides argument's default actions list to ensure that node ID
   * from URL is provided.
   */
  public function default_actions($which = NULL) {
    $defaults = parent::default_actions();
    unset($defaults['ignore'], $defaults['not found'], $defaults['summary'], $defaults['empty']);

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Validates that the argument works.
   */
  public function validate_arg($arg) {
    if (!parent::validate_arg($arg)) {
      return FALSE;
    }

    if (!empty($this->options['break_phrase'])) {
      views_break_phrase($this->argument, $this);
    }
    else {
      $this->value = array($this->argument);
    }
    $this->view->nids = $this->value;
    return TRUE;
  }
  
  /**
   * Override the behavior of title(). Get the title of the node.
   */
  function title_query() {
    $titles = array();
  
    $result = db_query("SELECT n.title FROM {node} n WHERE n.nid IN (:nids)", array(':nids' => $this->value));
    foreach ($result as $term) {
      $titles[] = check_plain($term->title);
    }
    return $titles;
  }

  /**
   * Builds the query.
   */
  public function query($group_by = FALSE) {
    // Since the view could have multiple nid arguments, load each node
    // and populate the $text variable with node titles and bodies.
    $boolean = !empty($this->options['boolean_mode']);
    $text = '';
    foreach ($this->value as $nid) {
      $node = node_load($nid);
      if (isset($node->title) && isset($node->body) && isset($node->body[$node->language])) {
        // Remove punctuation from the title.
        $title = preg_replace('/[^a-z0-9 _-]+/i', '', $node->title);

        // Increase the relevancy of words in the node title if option is selected.
        if ($boolean && !empty($this->options['increase_title']) && isset($this->options['title_operator'])) {
          $title = self::alter_node_title($title, $this->options['title_operator']);
        }
        // Strip tags and add slashes only to the body before adding the title.
        $body = trim(addslashes(strip_tags($node->body[$node->language][0]['value'])));
        $text .= " $title $body";
      }
    }

    if ($boolean && !empty($this->options['enable_custom_operators']) && !empty($this->options['custom_operators'])) {
      $replacements = array_map('trim', explode('|', $this->options['custom_operators']));
      $words = array_map('similar_handler_argument_nid::get_search_word', $replacements);
      $text = preg_replace($words, $replacements, $text);
    }

    $text = trim($text);
    $this->ensure_my_table();

    $selects = array();
    if (db_table_exists('field_data_body')) {
      $body_table = $this->query->add_table('field_data_body');
      $selects[] = "$body_table.body_value";
    }

    // Add more MATCH select statements for extra fields if enabled.
    if ($boolean && module_exists('field') && !empty($this->options['include_fields'])) {
      $indices = similar_get_indices();
      if (empty($this->options['included_fields'])) {
        foreach ($indices as $table => $indexed) {
          $this->add_similar_table($table, $indexed, $selects);
        }
      }
      else {
        foreach ($this->options['included_fields'] as $table => $field) {
          if (isset($indices[$table])) {
            $this->add_similar_table($table, $indexed, $selects);
          }
        }
      }
    }

    // An empty string is passed for the table to bypass the table alias.
    $fields = count($selects) > 1 ? implode(", ", $selects) : array_pop($selects);
    $query = $boolean ? "MATCH($fields) AGAINST('$text' IN BOOLEAN MODE)" : "MATCH($fields) AGAINST('$text')";
    $this->query->add_field('', $query, 'score');

    // Exclude the current node(s).
    if (count($this->value) > 1) {
      $this->query->add_where(0, 'node.nid', $this->value, 'NOT IN');
    }
    else {
      $this->query->add_where(0, 'node.nid', $this->value[0], '<>');
    }

    // Add a node_access tag to prevent conflict with node_access module.
    $this->query->add_tag('node_access');

    // Add a Similar Entries tag to indicate that filters and sort handlers are valid.
    $this->query->add_tag('similar_entries');
  }

  /**
   * Adds a table to the Similar Entries query.
   *
   * @param $table
   *   The table being added. This value comes from a key in similar_get_indices().
   * @param $indexed
   *   An array of fields belonging to the table being added. These columns have
   *   already been indexed by Similar Entries.
   * @param $selects
   *   An array of select fields that will be used to build the select query.
   * @see similar_handler_argument_nid::query()
   */
  private function add_similar_table($table, $indexed, &$selects) {
    if (db_table_exists($table) && !empty($indexed)) {
      $alias = $this->query->add_table($table);
      if (count($indexed) > 1) {
        $selects[] = "$alias." . implode(", $alias.", $indexed);
      }
      elseif (count($indexed) == 1) {
        $field = array_pop($indexed);
        $selects[] = "$alias.$field";
      }
    }
  }

  /**
   * Converts a user-defined word with operator into a search word for use
   * in preg_replace().
   *
   * @see similar_handler_argument_nid::query()
   */
  public static function get_search_word($word) {
    return '/\b'. trim($word, '+-<>~()"') .'\b/i';
  }

  /**
   * Alters the node title's relevance for boolean searches.
   *
   * @param $title
   *   The node title with punctuation removed.
   * @param $operator
   *   The operator to apply to the node title.
   * @see similar_handler_argument_nid::query()
   */
  private static function alter_node_title($title, $operator) {
    switch ($operator) {
      case 'increase':
        return '>'. implode(' >', explode(' ', $title));
      case 'decrease':
        return '<'. implode(' <', explode(' ', $title));
      case 'require_all':
        return '+'. implode(' +', explode(' ', $title));
      case 'require_some':
        return "+($title)";
      case 'require_complete':
        return '"'. $title .'"';
      case 'exclude_all':
        return '-'. implode(' -', explode(' ', $title));
      case 'exclude_some':
        return "-($title)";
      case 'exclude_complete':
        return '-("'. $title .'")';
      default:
        return $title;
    }
  }

}
