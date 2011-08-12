<?php

/**
 * @file
 * Defines the score filter handler for Similar entries module.
 */

/**
 * Defines the score filter handler.
 */
class similar_handler_filter_similarity extends views_handler_filter_numeric {

  /**
   * Defines default values for the operator and value.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['operator'] = array('default' => '>');
    $options['value'] = array('default' => '1');
    return $options;
  }

  /**
   * Alters the settings form to make the value a select list instead of text.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // The between/not between operators don't make sense for relevance.
    unset($form['operator']['#options']['between'], $form['operator']['#options']['not between']);
    $form['value']['#type'] = 'select';
    $form['value']['#description'] = t('Choosing a higher number will further limit the amount of results and provide only results with a higher similarity rating.');
    $form['value']['#options'] = array('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0');
    $form['value']['#default_value'] = $this->options['value'];
  }

  /**
   * Inserts a having clause into the query.
   */
  public function query() {
    $this->ensure_my_table();
    $this->query->add_having('nid', 'score', $this->value, $this->operator);
  }

}
